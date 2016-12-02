/**
 * 	@file: function_data.cpp
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */
 
#include <iostream>

#include "function_data.h"
#include "helpers.h"

FunctionData::FunctionData()
{
    // Nothing
}


FunctionData::FunctionData(std::string function_name, short return_type)
{
    this -> function_name = function_name;
    this -> return_type = return_type;
    
    filled_symbol_table = false;
}


void FunctionData::fill_symbol_table(void* base_pointer)
{
    this -> base_pointer = (long)base_pointer;
    filled_symbol_table = true;
    
    // TODO
    
}


bool FunctionData::has_symbol_table()
{
    return filled_symbol_table;
}


void FunctionData::add_parameters(std::string parameter, int idx)
{
    all_parameters[parameter] = parameter_types[idx];
}


void FunctionData::add_variables(std::string variable, short type)
{
    all_variables[variable] = type;
}


void FunctionData::add_variable_offsets(std::string variable, long offset)
{
    variable_offsets[variable] = offset;
    all_offsets.insert(offset);
}


void FunctionData::add_parameter_types(short type)
{
    parameter_types.push_back(type);
}


void FunctionData::add_start_line(int start_line)
{
    this -> start_line = start_line;
}
    
        
void FunctionData::add_end_line(int end_line)
{
    this -> end_line = end_line;
}


void FunctionData::add_base_pointer(long base_pointer)
{
    this -> base_pointer = base_pointer;
}


void FunctionData::add_first_instruction_line(int first_instruction_line)
{
    this -> first_instruction_line = first_instruction_line;
}


long FunctionData::get_base_pointer()
{
    return base_pointer;
}


short FunctionData::get_type(const std::string& variable)
{
    auto it = all_variables.find(variable);
    
    if (it != all_variables.end())
    {
        return it -> second;
    }
    
    return 42;
}

        
long FunctionData::get_offset(const std::string& variable)
{
    auto it = variable_offsets.find(variable);

    if (it != variable_offsets.end())
    {
        return it -> second;
    }
    
    return -42;
}


long FunctionData::get_variable_address(const std::string& variable)
{
    auto it = variable_offsets.find(variable);

    if (it != variable_offsets.end())
    {
        return base_pointer - it -> second;
    }
    
    return -42;
}

        
std::string FunctionData::get_function_name()
{
    return function_name;
}


int FunctionData::get_start_line()
{
    return start_line;
}


int FunctionData::get_first_instruction_line()
{
    return first_instruction_line;
}


bool FunctionData::lies_inside_function(int line_number)
{
    return (line_number >= start_line && line_number <= end_line);
}


bool FunctionData::is_variable(const std::string& potential_variable)
{
    if (all_variables.find(potential_variable) != all_variables.end())
    {
        return true;
    }
    
    return all_parameters.find(potential_variable) != all_parameters.end();
}



bool FunctionData::is_already_added(long offset)
{
    return (all_offsets.find(offset) != all_offsets.end());
}


void FunctionData::print_function_data(bool complete_function)
{
    print_function_name();
    print_return_type();
    print_variables();

    if (complete_function)
    {
        print_variable_offsets();
        print_parameters();
        print_line_info();
    }
    
    std::cout << "\n";
}


void FunctionData::print_function_name()
{
    std::cout << "----------------------------------------------------------\n";
    std::cout << "The name of the function is : " << function_name << "\n";
}


void FunctionData::print_return_type()
{
    std::cout << "The return type is : ";
    Helpers::print_type_string(return_type);
    std::cout << "\n";
}


void FunctionData::print_parameters()
{
    std::cout << "The parameter(s) of " << function_name << " is/are :\n";
    for (auto it = all_parameters.begin(); it != all_parameters.end(); ++it)
    {
        std::cout << "\t" << it -> first << "\t:\t";
        Helpers::print_type_string(it -> second);
        std::cout << "\n";
    }
    
    std::cout << "\n";
}


void FunctionData::print_variables()
{
    std::cout << "The variable(s) of " << function_name << " is/are :\n";
    for (auto it = all_variables.begin(); it != all_variables.end(); ++it)
    {
        std::cout << "\t" << it -> first << "\t:\t";
        Helpers::print_type_string(it -> second);
        std::cout << "\n";
    }
    
    std::cout << "\n";
}


void FunctionData::print_variable_offsets()
{
    std::cout << "The variable offset(s) of " << function_name << " is/are :\n";
    for (auto it = variable_offsets.begin(); it != variable_offsets.end(); ++it)
    {
        std::cout << "\t" << it -> first << "\t:\t" << it -> second << "\n";
    }
    
    std::cout << "\n";
}


void FunctionData::print_line_info()
{
    std::cout << function_name << " starts on line " << start_line << " and ends on line " << end_line << "\n";
}

