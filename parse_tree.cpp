/**
 * 	@file: parse_tree.cpp
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */

#include <unistd.h>
#include <iostream>
#include <cstring>

#include "parse_tree.h"
#include "helpers.h"
#include "child_helpers.h"

void ParseTree::initialize(const std::string& filename, const std::string& object_file)
{
    temporary_executable = Helpers::generate_random_string();
    generate_parse_tree(filename, object_file);
    Helpers::remove_file(temporary_executable);
}


void ParseTree::print_all_function_data(bool print_everything)
{
    for (int idx = 0; idx < function_data.size(); ++idx)
    {
        function_data[idx].print_function_data(print_everything);
    }   
}


FunctionData& ParseTree::get_function(int function_idx)
{
    return function_data[function_idx];
}


void ParseTree::add_start_line(int function_idx, int start_line)
{
    function_data[function_idx].add_start_line(start_line);
}

        
void ParseTree::add_end_line(int function_idx, int end_line)
{
    function_data[function_idx].add_end_line(end_line);
}


void ParseTree::add_first_instruction_line(int function_idx, int first_instruction_line)
{
    function_data[function_idx].add_first_instruction_line(first_instruction_line);
}


bool ParseTree::is_already_added(int function_idx, long offset)
{
    return function_data[function_idx].is_already_added(offset);
}

        
void ParseTree::add_variable_offsets(int function_idx, const std::string& variable, long offset)
{
    function_data[function_idx].add_variable_offsets(variable, offset);
}


int ParseTree::get_start_line(int function_idx)
{
    return function_data[function_idx].get_start_line();
}


int ParseTree::get_first_instruction_line(int function_idx)
{
    return function_data[function_idx].get_first_instruction_line();
}
        

std::string ParseTree::get_function_name(int function_idx)
{
    return function_data[function_idx].get_function_name();
}
        

void ParseTree::add_parameters(int function_idx, const std::string& parameter, int count)
{
    function_data[function_idx].add_parameters(parameter, count);
}


bool ParseTree::is_variable(int function_idx, const std::string& variable)
{
    return function_data[function_idx].is_variable(variable);
}


bool ParseTree::is_function(const std::string& potential_function)
{
    return function_to_idx.find(potential_function) != function_to_idx.end();
}


int ParseTree::get_start_line(const std::string& function)
{
    int function_idx = function_to_idx[function];
    return function_data[function_idx].get_start_line();
}


int ParseTree::get_first_instruction_line(const std::string& function)
{
    if (!is_function(function))
    {
        return -1;
    }

    int function_idx = function_to_idx[function];
    return function_data[function_idx].get_first_instruction_line();
}


void ParseTree::fill_symbol_table(const std::string& function, void* base_pointer)
{
    int function_idx = function_to_idx[function];
    function_data[function_idx].fill_symbol_table(base_pointer);
}

        
bool ParseTree::has_symbol_table(const std::string& function)
{
    int function_idx = function_to_idx[function];
    return function_data[function_idx].has_symbol_table();
}


short ParseTree::get_type(const std::string& function, const std::string& variable)
{
    int function_idx = function_to_idx[function];
    return function_data[function_idx].get_type(variable);
}


long ParseTree::get_variable_address(const std::string& function, const std::string& variable)
{
    int function_idx = function_to_idx[function];
    return function_data[function_idx].get_variable_address(variable);
}


std::string ParseTree::get_function_from_line(int line_number)
{
    for (int idx = 0; idx < function_data.size(); ++idx)
    {
        if (function_data[idx].lies_inside_function(line_number))
        {
            return function_data[idx].get_function_name();
        }
    }
}


void ParseTree::generate_parse_tree(const std::string& filename, const std::string& object_file)
{
    int parse_tree_pipe[2];
    pipe(parse_tree_pipe);
    
    pid_t parse_tree = fork();
    
    if (parse_tree == 0)
    {
        const char* parse_tree_args[7];
        parse_tree_args[0] = "g++";
        parse_tree_args[1] = "-fdump-tree-original=stdout";
        parse_tree_args[2] = &filename[0];
        parse_tree_args[3] = "-w";
        parse_tree_args[4] = "-o";
        parse_tree_args[5] = &temporary_executable[0];
        parse_tree_args[6] = NULL;
        
        ChildHelper::redirect_pipe_and_exec(parse_tree_args, parse_tree_pipe, STDOUT_FILENO);
    }
    
    ChildHelper::create_timeout(parse_tree, parse_tree_pipe, "Terminated --- Input File Is Too Big", temporary_executable, object_file, false, 0);
    
    close(parse_tree_pipe[1]);
    
    char str[65536];
    str[0] = 0;
	
	int bytesread = read(parse_tree_pipe[0], str, sizeof(str));	
	str[bytesread] = 0;
	close(parse_tree_pipe[0]);
	
	parse_the_tree(str, bytesread);
}


void ParseTree::parse_the_tree(char* str, int len)
{
    char* pattern = (char*)";; Function";           // Bad practice???
    std::vector<int> positions = Helpers::kmp(pattern, str);
    
    for (int idx = 0; idx < positions.size(); ++idx)
    {
        // If there are no variables, we don't want to continue past the end of the function
        int stop_idx = (idx != positions.size() - 1) ? positions[idx + 1] : len;
        parse_function(positions[idx], str, stop_idx);
    }
}


void ParseTree::parse_function(int idx, char* str, int stop_idx)
{
    int start_idx = idx + 1;
    int current_idx = idx;
    
    while (str[++current_idx] != '(');
    
    int function_idx = current_idx;
    
    while (str[--function_idx] != ' ');
    
    char function[current_idx - function_idx];
    strncpy(function, &str[function_idx + 1], current_idx - function_idx - 1);
    function[current_idx - function_idx - 1] = 0;
    
    // Discard the compiler internal generated functions
    if (function[0] == '_' && function[1] == '_')
    {
        return;
    }
    
    char return_type[function_idx - start_idx + 1];
    strncpy(return_type, &str[start_idx], function_idx - start_idx);
    return_type[function_idx - start_idx] = 0; 
    
    short return_enum = Helpers::get_type(return_type);
    
    FunctionData fd(function, return_enum);
    
    fill_parameter_data(current_idx, str, fd);
    fill_variable_data(current_idx, str, stop_idx, fd);
    
    function_data.push_back(fd);
    
    // Set the function to index map to the most recent index
    function_to_idx[function] = function_data.size() - 1;
}


void ParseTree::fill_parameter_data(int idx, char* str, FunctionData& fd)
{
    int parameter_idx = idx;
    
    while (str[++parameter_idx] != ')');
    
    // No parameters
    if (parameter_idx - idx == 1)
    {
        return;
    }
    
    char all_parameters[parameter_idx - idx];
    strncpy(all_parameters, &str[idx + 1], parameter_idx - idx - 1);
    all_parameters[parameter_idx - idx - 1] = 0;
    
    int start_idx = idx;
    int end_idx = idx;
    
    while (end_idx != parameter_idx)
    {
        while (end_idx != parameter_idx && str[++end_idx] != ',');
        
        char parameter_type[end_idx - start_idx];
        strncpy(parameter_type, &str[start_idx + 1], end_idx - start_idx - 1);
        parameter_type[end_idx - start_idx - 1] = 0;
        
        short parameter_enum = Helpers::get_type(parameter_type);
        fd.add_parameter_types(parameter_enum);
        
        start_idx = end_idx + 1;
    } 
}


void ParseTree::fill_variable_data(int idx, char* str, int stop_idx, FunctionData& fd)
{
    while (++idx != stop_idx && str[idx] != '{');
    
    // Found no parameters
    if (idx == stop_idx)
    {
        return;
    }
    
    recursive_fill_data(idx, str, stop_idx, fd);   
}


void ParseTree::recursive_fill_data(int& idx, char* str, int stop_idx, FunctionData& fd)
{
    int stack = 0;
    
    while (idx < stop_idx)
    {
        // Everytime we hit a { we recurse deeper unless there are no variables in that scope
        if (str[idx] == '{')
        {
            ++idx;
            ++stack;
            
            int end_idx = idx;
            
            // Found new variables so we extract them
            if (found_end_idx(str, end_idx, stop_idx))
            {
                int end;
                while (idx < end_idx)
                {
                    if (str[idx] == '\n' && str[idx + 1] == ' ')
                    {
                        while (str[++idx] == ' ');
                    }
                    
                    end = idx;
                    while (str[++end] != ';');
                    
                    int variable_idx = end;
                    
                    // Check if there is a equal sign 
                    while (variable_idx != idx && str[--variable_idx] != '=');
                    
                    if (str[variable_idx] == '=')
                    {
                        parse_line_for_variables(idx, variable_idx - 1, str, fd);
                    }
                    else
                    {
                        parse_line_for_variables(idx, end, str, fd);
                    }
                    
                    idx = end + 1;
                }
            }
            else if (found_new_bracket(str, end_idx, stop_idx))
            {
                // Found a { instead so we recurse into it
                ++stack;
                recursive_fill_data(end_idx, str, stop_idx, fd);
            }
            else
            {
                // Found a } so we decrease our stack counter
                --stack; 
                
                if (str[end_idx] == '}')
                {
                    if (stack == 0)
                    {
                        return;
                    }
                }
            }
            
            idx = end_idx;   
        } 
        
        // If we closed our bracket, we return
        if (str[idx] == '}')
        {
            --stack;
            
            if (stack == 0)
            {
                return;
            }
        }
        
        ++idx;
    }
}


bool ParseTree::found_end_idx(char* str, int& end_idx, int stop_idx)
{
    while (end_idx < stop_idx)
    {
        if (str[end_idx] == '\n' && str[end_idx + 1] == '\n')
        {
            return true;
        }
        
        if (str[end_idx] == '{')
        {
            return false;
        }
        
        if (str[end_idx] == '}')
        {
            return false;
        }
        
        if (str[end_idx] == '"')
        {
            while (str[++end_idx] != '"');
            ++end_idx;
        }
        
        ++end_idx;
    }
    
    return false;
}


bool ParseTree::found_new_bracket(char* str, int& end_idx, int stop_idx)
{
    while (end_idx != stop_idx)
    {
        if (str[end_idx] == '{')
        {
            return true;
        }
        
        if (str[end_idx] == '}')
        {
            return false;
        } 
        
        if (str[end_idx] == '"')
        {
            while (str[++end_idx] != '"');
            ++end_idx;
        }
        
        ++end_idx;
    }
    
    return false;
}


void ParseTree::parse_line_for_variables(int idx, int end_idx, char* str, FunctionData& fd)
{
    int begin_idx = end_idx;
    while (str[--begin_idx] != ' ');
    
    char variable[end_idx - begin_idx];
    strncpy(variable, &str[begin_idx + 1], end_idx - begin_idx - 1);
    variable[end_idx - begin_idx - 1] = 0;
    
    char type[begin_idx - idx + 1];
    strncpy(type, &str[idx], begin_idx - idx);
    type[begin_idx - idx] = 0;
    
    short type_enum = Helpers::get_type(type);
    
    fd.add_variables(variable, type_enum);
}
