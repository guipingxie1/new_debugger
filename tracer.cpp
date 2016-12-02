/**
 * 	@file: tracer.cpp
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */
 
#include <iostream>
#include <cstring>
#include <csignal>

#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/user.h>

#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/syscall.h>

// Used to exit child
#include <sys/prctl.h> 				

#include "tracer.h"
#include "helpers.h"


// Should change this in the future?
std::string Tracer::static_compiled_object;
std::string Tracer::static_object_file;


void Tracer::cleanup_signal_handler(int signal)
{
    remove(&Tracer::static_compiled_object[0]);
    remove(&Tracer::static_object_file[0]);
    exit(0);
}


void* Tracer::get_instruction_pointer(pid_t pid) 
{
	return (void*)ptrace(PTRACE_PEEKUSER, pid, 8 * RIP, NULL);
}


void Tracer::set_instruction_pointer(pid_t pid, void* address) 
{
	ptrace(PTRACE_POKEUSER, pid, 8 * RIP, address);
}


void* Tracer::get_base_pointer(pid_t pid) 
{
	return (void*)ptrace(PTRACE_PEEKUSER, pid, 8 * RBP, NULL);
}


void* Tracer::get_stack_pointer(pid_t pid) 
{
	return (void*)ptrace(PTRACE_PEEKUSER, pid, 8 * RSP, NULL);
}


void* Tracer::get_return_address(pid_t pid, void* stack_pointer) 
{
	return (void*)ptrace(PTRACE_PEEKTEXT, pid, stack_pointer, NULL);
}


long Tracer::get_data(pid_t pid, void* address) 
{
	return ptrace(PTRACE_PEEKDATA, pid, address, NULL);
}


void Tracer::set_data(pid_t pid, void* address, void* data) 
{
	ptrace(PTRACE_POKEDATA, pid, address, data);
}


bool Tracer::breakpoint_already_exists(void* address)
{
    return address_to_breakpoint_info.find(address) != address_to_breakpoint_info.end();
}


bool Tracer::set_breakpoint(pid_t pid, void* address, bool is_new_breakpoint)
{
    // Check that the new breakpoint does not already exist
    if (is_new_breakpoint)
    {
        if (breakpoint_already_exists(address))
        {
            return false;
        }
    }

    // Inject INT3 = 0xCC which is the breakpoint interrupt    
    long former_data = get_data(pid, address);
	long new_data = former_data;
	new_data = (new_data & ~0xFF) | 0xCC;
	set_data(pid, address, (void*)new_data);
	
	if (is_new_breakpoint)
	{
	    BreakpointInfo break_info(former_data);
	    address_to_breakpoint_info[address] = break_info;
	}
	
	return true;
}


void Tracer::set_breakpoint_at_main(pid_t pid)
{
    // TODO : Need to change to last instruction address of main (put in parse_objdump.cpp)
    set_breakpoint_at_function(pid, "main");
}


void Tracer::set_breakpoint_at_function(pid_t pid, const std::string& function)
{
    int ignore;
    
    // NOOOO - Want to set the breakpoint at a line after the start of the function?
    int breakpoint_line = parsed_object_dump.get_first_instruction_line(function);
    
    if (breakpoint_line == -1)
    {   
        std::cerr << "The provided input : '" << function << "' is not a valid function\n";
        return;
    }
    
    long address = decoded_line.get_address_from_line(breakpoint_line, ignore);
    bool success = set_breakpoint(pid, (void*)address, true);
    
    if (success)
    {
        std::cerr << "Breakpoint set at address " << (void*)address << " on line " << breakpoint_line << "\n";
    }
    else
    {
        std::cerr << "Breakpoint already set at '" << function << "', request ignored\n";
    }
}


void Tracer::set_breakpoint_at_line(pid_t pid, int line_number)
{
    if (line_number < 0)
    {
        std::cerr << "Line number provided : " << line_number << " is negative\n";
        return;
    }

    int returned_line;
    long address = decoded_line.get_address_from_line(line_number, returned_line);
    
    if (address == 0)    
    {
        std::cerr << line_number << " is past the end of the file\n";
        return;
    }
    
    bool success = set_breakpoint(pid, (void*)address, true);
    
    if (success)
    {
        if (returned_line == line_number)
        {
            std::cerr << "Breakpoint set at address " << (void*)address << " on line " << line_number << "\n";
        }
        else
        {
            std::cerr << "Breakpoint set at address " << (void*)address << " on a later line " << returned_line << "\n";
        }
    }
    else
    {
        if (returned_line == line_number)
        {
            std::cerr << "Breakpoint already set at " << line_number << ", request ignored\n";
        }
        else
        {
            std::cerr << "Breakpoint already set at a later line " << returned_line << ", request ignored\n";
        }
    }
}


void Tracer::build_symbol_table(pid_t pid, const std::string& function)
{
    void* base_pointer = get_base_pointer(pid);
    parsed_object_dump.fill_symbol_table(function, base_pointer);
}


// NOT USED
void Tracer::print_variable(pid_t pid, const std::string& variable, const std::string& function)
{
    // TODO : no need???
}


void Tracer::handle_breakpoint(pid_t pid)
{
    // TODO : may have some errors here?
    // Fixed one of them
    void* address = get_instruction_pointer(pid);
    void* former_address = (void*)((long)address - 1);
    set_data(pid, former_address, (void*)address_to_breakpoint_info[former_address].get_data());
    set_instruction_pointer(pid, former_address);
    
    int line_number = decoded_line.get_line_number_from_address((long)former_address);
    std::cerr << "Hit breakpoint on line " << line_number << "\n\t";
    parsed_object_dump.print_line(line_number);
    
    // TODO : need to reset breakpoints
}


void Tracer::remove_breakpoint(pid_t pid, const std::string line_number)
{
    if (Helpers::is_number(line_number))
    {
        if (line_number.length() > 8)
        {
            std::cerr << "Line number provided : " << line_number << " is too big\n";
        }
        else
        {
            // TODO
        }       
    }
    else
    {
        std::cerr << "Invalid remove breakpoint request : " << line_number << ". Need to specify a valid line number\n";
    }   
}


void Tracer::handle_stepping(pid_t pid, int former_line_number)
{
    // TODO
    void* instruction_address = get_instruction_pointer(pid);
}


void Tracer::print_introduction()
{
    // TODO
}


void Tracer::print_help_section()
{
    // TODO
    std::cerr << "Not completed yet!\n";
}


void Tracer::print_all_breakpoints()
{
    // TODO
}


bool Tracer::confirm_exit()
{
    std::string input;
    
    while (true) 
    {
		std::cerr << "Are you sure you want to quit? ('y'/'yes' or 'n'/'no') ";
		getline(std::cin, input);
		
		if (input.compare("y") == 0 || input.compare("yes") == 0) 
		{			
			return true;
		}
		else if (input.compare("n") == 0 || input.compare("no") == 0) 
		{		
			return false;
		}
		else 
		{
			std::cerr << "Invalid answer: '" << input << "'\n";
		}
	}
	
	return false;
}


void Tracer::set_current_function(pid_t pid)
{
    void* instruction_address = get_instruction_pointer(pid);
    int line_number = decoded_line.get_line_number_from_address((long)instruction_address);
    
    if (line_number == -1)
    {
        return;
    }
    
    current_function = parsed_object_dump.get_function_from_line(line_number);
    
    if (!parsed_object_dump.has_symbol_table(current_function))
    {
        build_symbol_table(pid, current_function);
    }
}


// NOT USED
std::string Tracer::parse_command_for_variable(const std::string& input)
{
    int end_idx = input.find(" ");
    
    if (end_idx == -1)
    {
        std::cerr << "No variable provided\n";
        return std::string("");
    }
    
    std::string variable = input.substr(end_idx + 1);
    return variable;
}


void Tracer::handle_breakpoint_request(pid_t pid, const std::string break_position)
{
    // TODO
    
    if (break_position.empty())
    {
        std::cerr << "No position provided\n";
        return;
    }
    
    // TODO : Maybe break up the variables if more than one provided?
    
    if (isdigit(break_position[0]))
    {
        if (Helpers::is_number(break_position))
        {
            if (break_position.length() > 8)
            {
                std::cerr << "Line number provided : " << break_position << " is too big\n";
            }
            else
            {
                set_breakpoint_at_line(pid, stoi(break_position));
            }       
        }
        else
        {
            std::cerr << "Invalid breakpoint request : " << break_position << "\n";
        }   
    } 
    else if (isalpha(break_position[0]) || break_position[0] == '_')
    {
        // Get function name
        set_breakpoint_at_function(pid, break_position);
    }
    else
    {
        std::cerr << "Invalid breakpoint request : " << break_position << "\n";
    }
}


void Tracer::print_char_string(pid_t pid, long char_string_address)
{
    bool flag = true;
    long data;
    long offset = 0;
    char c;
    
    std::cerr << "\"";
    
    do
    {
        data = get_data(pid, (void*)(char_string_address + offset));
        for (int idx = 0; idx < 8; ++idx)
        {
            c = (char)data;
            
            if (c == 0)
            {
                flag = false;
                break;
            }
            else 
            {
                std::cerr << c;
                data >>= 8;
            }
        }
        
        offset += 8;
    }
    while (flag);
    
    std::cerr << "\"\n";
}


void Tracer::print_variable(pid_t pid, short type, long data)
{
    if (type == 0)
    {
        std::cerr << (int)data << "\n";
    }
    else if (type == 1)
    {
        std::cerr << (short)data << "\n";
    }
    else if (type == 2)
    {
        std::cerr << (long)data << "\n";
    }
    else if (type == 3)
    {
        float float_data;
        memcpy(&float_data, &data, sizeof(float));
        std::cerr << float_data << "\n";
    }
    else if (type == 4)
    {
        double double_data;
        memcpy(&double_data, &data, sizeof(double));
        std::cerr << double_data << "\n";
    }
    else if (type == 5)
    {
        std::cerr << "'" << (char)data << "'\n";
    }
    else if (type == 6)
    {
        print_char_string(pid, data);
    }
}


void Tracer::handle_print_variable(pid_t pid, const std::string variables)
{
    if (variables.empty())
    {
        std::cerr << "No variable provided\n";
        return;
    }
    
    // TODO : Maybe break up the variables if more than one provided?
    
    std::string variable = variables;
    
    short type = parsed_object_dump.get_type(current_function, variable);
    
    if (type == 42)
    {
        std::cerr << "Variable : '" << variable << "' is not in scope\n";
    }
    
    long stack_address = parsed_object_dump.get_variable_address(current_function, variable);
    long data = get_data(pid, (void*)stack_address);
    
    print_variable(pid, type, data);
}


void Tracer::parse_command(const std::string& input, std::string& command, std::string& additional_commands)
{
    int end_idx = input.find(" ");
    
    if (end_idx == -1)
    {
        command = std::string(input);
        additional_commands = std::string("");
        return;
    }
    
    command = input.substr(0, end_idx);
    additional_commands = input.substr(end_idx + 1);
}


bool Tracer::is_breakpoint(pid_t pid)
{
    void* instruction_address = get_instruction_pointer(pid);
    void* breakpoint_address = (void*)((long)instruction_address - 1);
    return address_to_breakpoint_info.find(breakpoint_address) != address_to_breakpoint_info.end();
}


void Tracer::handle_input_commands(pid_t pid)
{
    // TODO
    bool at_breakpoint = is_breakpoint(pid);
    if (at_breakpoint)
    {
        handle_breakpoint(pid);
    }
    else
    {
        // Is stepping through --- not at the very beginning
        
    }
    
    set_current_function(pid);
    
    std::cerr << "(debugger) ";
    std::string input;
    
    while (getline(std::cin, input))
    {
        Helpers::trim(input);
        std::string command;
        std::string additional_commands;
        parse_command(input, command, additional_commands);
        
        int command_enum = parsed_commands.get_debug_command(command);
        
        if (command_enum == 1)
        {
            // Run
        }
        else if (command_enum == 2)
        {
            // Continue
            break;
        }
        else if (command_enum == 3)
        {
            // Step
        }
        else if (command_enum == 4)
        {
            // Break
            handle_breakpoint_request(pid, additional_commands);
        }
        else if (command_enum == 5)
        {
            // Watch
        }
        else if (command_enum == 6)
        {
            remove_breakpoint(pid, additional_commands);
        }
        else if (command_enum == 7)
        {
            // Remove watch
        }
        else if (command_enum == 8)
        {
            // Point info
        }
        else if (additional_commands.empty() && command_enum == 9)
        {
            if (confirm_exit())
            {
                kill(pid, SIGKILL);
                break;
            }
        }
        else if (command_enum == 10)
        {
            handle_print_variable(pid, additional_commands);
        }
        else if (command_enum == 11)
        {
            print_help_section();
        }
        else
        {
            std::cerr << "The provided input : \"" << input << "\" is not valid\n";
        }
        
        std::cerr << "(debugger) ";
    }
    
    // Need to reset the breakpoint however we should not on step
    
    ptrace(PTRACE_CONT, pid, NULL, NULL);
}


Tracer::Tracer(int argc, char* argv[])
{
    compiled_file.initialize(argc, argv);
    std::string filename = compiled_file.get_file();
    std::string executable = compiled_file.get_compiled_object();
    std::string object_file = compiled_file.get_object_file();
    
    decoded_line.initialize(executable, object_file, filename);
    decoded_line.print_address_to_line();
    
    parsed_object_dump.initialize(executable, object_file, filename);
    parsed_object_dump.print_all_function_data();
    
    parsed_commands.initialize(executable, object_file);
    
    current_function = std::string("main");
 
    // Actually never used
    breakpoint_number = 0;
    watchpoint_number = 0;
    
    Tracer::static_compiled_object = executable;
    Tracer::static_object_file = object_file;
    
    std::signal(SIGINT, Tracer::cleanup_signal_handler);
    std::signal(SIGSEGV, Tracer::cleanup_signal_handler);
    
    run_debugger(argv);
}   


Tracer::~Tracer()
{
    compiled_file.remove_compiled_objects();
}


void Tracer::run_debugger(char* argv[])
{
    pid_t trace_child = fork();
    
    if (trace_child == 0)
    {
        // Kill child if parent terminates
        prctl(PR_SET_PDEATHSIG, SIGHUP);
	
	    // Begin tracing child and run the program
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		std::string executable = compiled_file.get_compiled_object();
		
		char execute_command[3 + executable.length()];
		execute_command[0] = '.';
		execute_command[1] = '/';
		strncpy(&execute_command[2], &executable[0], executable.length());
		execute_command[2 + executable.length()] = 0;
		
		int exec_result = execlp(execute_command, execute_command, (char*)0);
		
		if (exec_result == -1)
		{
		    std::cerr << "\n\t\e[1m Cannot execute the object produced by the file \e[0m\n\n";
		    exit(8);
		}
    }
    
    int	status;
    waitpid(trace_child, &status, 0);
    
    if (WIFEXITED(status) && WEXITSTATUS(status) == 8) 
    {
    	return;
    }
    
	// Initial stop
	if (WIFSTOPPED(status)) 
	{
		if (WSTOPSIG(status) == SIGTRAP) 	
		{	
			print_introduction();
			//handle_input_commands(trace_child);
			set_breakpoint_at_main(trace_child);
			set_breakpoint_at_line(trace_child, 39);
			ptrace(PTRACE_CONT, trace_child, NULL, NULL);
		}	
	}
/*	
	// Stop at main
	waitpid(trace_child, &status, 0);
	
	if (WIFSTOPPED(status)) 
	{
		if (WSTOPSIG(status) == SIGTRAP) 
		{		
			void* instruction_address = get_instruction_pointer(trace_child);
            std::cout << instruction_address << "\n";
            
            // Restore main function
		    handle_breakpoint(trace_child, instruction_address, true);
		    
		    // Build main symbol table
		    build_symbol_table(trace_child, current_function);
		
			// Single step to next instruction 
			ptrace(PTRACE_SINGLESTEP, trace_child, NULL, NULL);
		}
	}
*/	
/*	
	waitpid(trace_child, &status, 0);
	void* instruction_address = get_instruction_pointer(trace_child);
	std::cout << instruction_address << "\n";
*/	
	do {
		waitpid(trace_child, &status, 0);
		
		if (WIFEXITED(status) || (WIFSIGNALED(status) && WTERMSIG(status) == SIGKILL)) 
	    {
			break;
        }

		if (WIFSTOPPED(status)) 
		{
			if (WSTOPSIG(status) == SIGTRAP) 
			{
				handle_input_commands(trace_child);
			}
		}
	}
	while (!WIFEXITED(status));
}
