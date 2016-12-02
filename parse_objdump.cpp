/**
 * 	@file: parse_objdump.cpp
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */
 
#include <unistd.h> 
#include <cstring>
#include <iostream>
#include <vector>

#include "parse_objdump.h"
#include "child_helpers.h"
#include "helpers.h"


void ParsedObjdump::initialize(const std::string& executable, const std::string& object_file, const std::string& filename)
{
    parsed_tree.initialize(filename, object_file);
    
    parsed_file.initialize(filename);

    this -> executable = executable;
    this -> object_file = object_file;
    
    construct_map(filename);
} 


void ParsedObjdump::print_line(int line_number)
{
    parsed_file.print_line(line_number);
}


void ParsedObjdump::print_all_function_data()
{
    parsed_tree.print_all_function_data(true);
}


int ParsedObjdump::get_first_instruction_line(const std::string& function)
{
    return parsed_tree.get_first_instruction_line(function);
}


void ParsedObjdump::fill_symbol_table(const std::string& function, void* base_pointer)
{
    parsed_tree.fill_symbol_table(function, base_pointer);
}

        
bool ParsedObjdump::has_symbol_table(const std::string& function)
{
    return parsed_tree.has_symbol_table(function);
}


std::string ParsedObjdump::get_function_from_line(int line_number)
{
    return parsed_tree.get_function_from_line(line_number);
}


short ParsedObjdump::get_type(const std::string& function, const std::string& variable)
{
    return parsed_tree.get_type(function, variable);
}


long ParsedObjdump::get_variable_address(const std::string& function, const std::string& variable)
{
    return parsed_tree.get_variable_address(function, variable);
}


void ParsedObjdump::construct_map(const std::string& filename)
{
    int objdump_pipe[2];
    pipe(objdump_pipe);
    
    pid_t objdump_pid = fork();
    
    if (objdump_pid == 0)
    {
        const char* objdump_args[8];
        objdump_args[0] = "objdump";
        objdump_args[1] = "-d";
        objdump_args[2] = "-l";
        objdump_args[3] = "-C";
        objdump_args[4] = "-j";
        objdump_args[5] = ".text";
        objdump_args[6] = &object_file[0];  
        objdump_args[7] = NULL;
        
        ChildHelper::redirect_pipe_and_exec(objdump_args, objdump_pipe, STDOUT_FILENO);
    }
        
    ChildHelper::create_timeout(objdump_pid, objdump_pipe, "Terminated --- Input File Is Too Big", executable, object_file, false, 0);
    
    close(objdump_pipe[1]);
    
    char str[65536];
    str[0] = 0;
	
	int bytesread = read(objdump_pipe[0], str, sizeof(str));	
	str[bytesread] = 0;
	close(objdump_pipe[0]);
	
    std::string parsed_filename = Helpers::get_parsed_filename(filename);
	
	parse_objdump(parsed_filename, str);
}


void ParsedObjdump::parse_objdump(const std::string& filename, char* str)
{
    // Construct pattern we want to match with
    int length = filename.length();
    char pattern[length + 2];
    strncpy(pattern, &filename[0], length);
    pattern[length] = ':';
    pattern[length + 1] = 0;
    
    std::vector<int> positions = Helpers::kmp(pattern, str);
    
	int start = 0;
	int end = 0;
	int idx = positions[start];
	bool completed_function = false;
	
	int count = 0;
	// Loop through str and find the blocks that correspond to a function
	while (end < positions.size() || !completed_function)
	{
	    if (str[idx] == 0 || (str[idx] == '\n' && str[idx + 1] == '\n'))
	    {
	        parse_function(positions, str, start, end - 1, count);
            
            ++count;
            start = end;
            completed_function = true;
            
            // Buffer zone hack so we do not need conditionals while parsing
            if (end == positions.size())
            {
                positions.push_back(idx);
                ++end;
            }
	    }
	    else 
	    {
	        completed_function = false;
	        
	        if (end < positions.size() && idx > positions[end])
	        {
	            ++end;
	        }
	    }
	    
	    ++idx;
	}
}


void ParsedObjdump::parse_function(const std::vector<int>& positions, char* str, int start, int end, int function_idx)
{
    int start_line = get_line_number(str, positions[start]);
    
    int first_instruction_line;
    if (start == end || start + 1 == end)
    {   
        first_instruction_line = start;
    }
    else
    {
        first_instruction_line = get_line_number(str, positions[start + 1]);
    }
    
    int end_line = get_line_number(str, positions[end]);
    
    parsed_tree.add_start_line(function_idx, start_line);
    parsed_tree.add_end_line(function_idx, end_line);
    parsed_tree.add_first_instruction_line(function_idx, first_instruction_line);
    extract_function_info(positions, str, start, end, function_idx);
}


int ParsedObjdump::get_line_number(char* str, int index)
{
    int start = index;
    
    while (str[index] != '\n' && str[index] != ' ')
    {
        ++index;
    }
    
    char number[index - start + 1];
    strncpy(number, &str[start], index - start);
    number[index - start] = 0;
    
    return atoi(number);
}


void ParsedObjdump::extract_function_info(const std::vector<int>& positions, char* str, int start, int end, int function_idx)
{
    int idx = positions[start];
    int end_idx = positions[++start];
    int line_number;
    
    extract_parameters(str, idx, end_idx, function_idx);
    
    while (start < end)
    {
        idx = positions[start];
        end_idx = positions[start + 1];
        
        // Find all instances of (%rbp)\n in the objdump
        std::vector<int> rbp_positions = Helpers::kmp((char*)BASE_POINTER, str, idx, end_idx);
        
        if (!rbp_positions.empty())
        {
            line_number = get_line_number(str, positions[start]);
            parse_line(line_number, str, rbp_positions, function_idx);
        }
        
        ++start;
    }
}


void ParsedObjdump::parse_line(int line_number, char* str, const std::vector<int>& rbp_positions, int function_idx)
{
    const std::string line = parsed_file.get_line(line_number);
    
    int idx = rbp_positions.size() - 1;
    int line_idx = line.length() - 1;
    
    while (line_idx != -1 && idx != -1)
    {
        if (is_valid_equal_sign(line, line_idx)) 
        {
            std::string variable = extract_variable(line, --line_idx, function_idx);
            long offset = extract_offset(str, rbp_positions[idx]);
            --idx;
            
            if (!parsed_tree.is_already_added(function_idx, offset))
            {
                parsed_tree.add_variable_offsets(function_idx, variable, offset);
            }
        }
        
        if (line_idx == -1)
        {
            return;
        }
        
        // Get rid of strings since it may contain = (ie string s = "1 + 1 = 2") 
        if (line[line_idx] == '\'')
        {
            while (line[--line_idx] != '\'');
            --line_idx;
        }
        
        if (line[line_idx] == '"')
        {
            while (line[--line_idx] != '"');
            --line_idx;
        }
        
        --line_idx;
    }
}


void ParsedObjdump::extract_parameters(char* str, int idx, int end_idx, int function_idx)
{
    std::vector<int> rbp_positions = Helpers::kmp((char*)BASE_POINTER, str, idx, end_idx);
        
    if (!rbp_positions.empty())
    {
        int start_line = parsed_tree.get_start_line(function_idx);    
        const std::string line = parsed_file.get_line(start_line);
        std::string function_name = parsed_tree.get_function_name(function_idx);
        
        int position = line.find(function_name) + function_name.length();
        int end = position;
        
        int count = 0;
        while (line[end] != ')')
        {
            if (line[end] == ',')
            {
                ++count;
            }
            
            ++end;
        }
        
        while (end != position)
        {
            int current = end;
            
            while (line[--current] == ' ');
            
            int parameter_idx = current;
            
            while (line[current] != ' ' && line[current] != '*' && line[current] != '&')
            {
                --current;
            }
            
            char parameter[parameter_idx - current + 1];
            strncpy(parameter, &line[current + 1], parameter_idx - current);
            parameter[parameter_idx - current] = 0;
            
            parsed_tree.add_parameters(function_idx, parameter, count);
            long offset = extract_offset(str, rbp_positions[count]);
            parsed_tree.add_variable_offsets(function_idx, parameter, offset);
            
            --count;
            
            while (line[--current] != ',' && current != position);
            end = current;
        }
    }
}


std::string ParsedObjdump::extract_variable(const std::string& line, int& line_idx, int function_idx)
{
    while (line[--line_idx] == ' ');
    
    int end_idx = line_idx;
    while (line_idx != -1 && is_valid_character(line[line_idx]))
    {
        --line_idx;
    }
    
    int length = end_idx - line_idx;
    std::string variable = line.substr(line_idx + 1, length);
    
    if (parsed_tree.is_variable(function_idx, variable))
    {
        return variable;
    }
    
    std::cerr << "\n\t\e[1m Error : Cannot parse file (file was too complicated) \e[0m\n\n";
    Helpers::remove_file(object_file);
    Helpers::remove_file(executable);
    exit(1);
}


bool ParsedObjdump::is_valid_equal_sign(const std::string& line, int line_idx)
{
    if (line[line_idx] == '=')
    {
        if (line_idx == 0)
        {
            return false;
        }
        
        if (line[line_idx - 1] == '=')
        {
            return false;
        }
        
        return (is_valid_character(line[line_idx - 1]) || line[line_idx - 1] == ' ');
    }
    
    return false;
}


bool ParsedObjdump::is_valid_character(char c)
{
    return (std::isalnum(c) || c == '_');
}


long ParsedObjdump::extract_offset(char* str, int idx)
{
    int end_idx = idx;
    
    while (str[--end_idx] != '(');
    
    int minus_idx = end_idx;
    
    while (str[--minus_idx] != '-');
    
    char offset[end_idx - minus_idx];
    strncpy(offset, &str[minus_idx + 1], end_idx - minus_idx - 1);
    offset[end_idx - minus_idx - 1] = 0;
    
    long hex_offset = std::stol(offset, NULL, 16);
    return hex_offset;
}
