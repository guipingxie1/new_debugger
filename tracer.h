/**
 * 	@file: tracer.h
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */

#include "parse_objdump.h"
#include "compile_file.h"
#include "decoded_line.h"
#include "parse_debug_commands.h"
#include "breakpoint_info.h"

#ifndef TRACER_H 
#define TRACER_H

// We assume the machine is 64 bit
class Tracer
{
    private:
        // Objects which hold the data required for debugging
        CompiledFile compiled_file;
        DecodedLine decoded_line;
        ParsedObjdump parsed_object_dump;
        ParsedCommands parsed_commands;
    
        std::string current_function;
        
        // Signal handler for Ctrl + C
        static void cleanup_signal_handler(int signal);
        static std::string static_compiled_object;
        static std::string static_object_file;
        
        // Used to keep track of the breakpoints
        int breakpoint_number;
        int watchpoint_number;
        
        // Need a better way to keep track of hitting a breakpoint
        bool reset_breakpoint;
        
        void* former_breakpoint_address;
        
        bool should_continue;
        
        int former_line_number;
        
        std::map< void*, BreakpointInfo > address_to_breakpoint_info;
        
    
        void* get_instruction_pointer(pid_t pid);
        
        void set_instruction_pointer(pid_t pid, void* address);
        
        void* get_base_pointer(pid_t pid);
        
        void* get_stack_pointer(pid_t pid);
        
        void* get_return_address(pid_t pid, void* stack_pointer);
        
        long get_data(pid_t pid, void* address);
        
        void set_data(pid_t pid, void* address, void* data); 
        
        void run_debugger(char* argv[]);
        
        void set_breakpoint_at_main(pid_t pid);
        
        void set_breakpoint_at_function(pid_t pid, const std::string& function);
        
        void set_breakpoint_at_line(pid_t pid, int line_number);
        
        bool set_breakpoint(pid_t pid, void* address, bool is_new_breakpoint);
        
        bool breakpoint_already_exists(void* address);
        
        bool is_breakpoint(pid_t pid);
        
        void build_symbol_table(pid_t pid, const std::string& function);
        
        void handle_breakpoint(pid_t pid);
        
        void handle_stepping(pid_t pid);
        
        void print_introduction();
        
        void print_help_section();
        
        void print_all_breakpoints();
        
        void print_all_watchpoints();
        
        void handle_first_input_commands(pid_t pid);
        
        void take_action(pid_t pid);
        
        void handle_input_commands(pid_t pid, bool is_first_stop);
        
        void print_variable(pid_t pid, const std::string& variable, const std::string& function);
        
        void set_current_function(pid_t pid);
        
        bool confirm_exit();
        
        void parse_command(const std::string& input, std::string& command, std::string& additional_commands);
        
        void handle_print_variable(pid_t pid, const std::string variables);
        
        std::string parse_command_for_variable(const std::string& input);
        
        void print_variable(pid_t pid, short type, long data);
                
        void print_char_string(pid_t pid, long char_string_address);
        
        void handle_breakpoint_request(pid_t pid, const std::string break_position);
        
        void remove_breakpoint(pid_t pid, const std::string line_number);
        
        void remove_breakpoint(pid_t pid, int line_number);
        
        bool step_into_breakpoint(pid_t pid);
        
        
        
    public:
        Tracer(int argc, char* argv[]);
        
        ~Tracer();
    
};

#endif   
