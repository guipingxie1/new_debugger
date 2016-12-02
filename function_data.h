/**
 * 	@file: function_data.h
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */

#ifndef FUNCTION_H 
#define FUNCTION_H 

#include <string>
#include <vector>
#include <map>
#include <set>

class FunctionData
{
    private:
        // Following three maps from variable to type and the offset
        std::map< std::string, short > all_variables;
        
        std::map< std::string, short > all_parameters;
    
        std::map< std::string, long > variable_offsets;
        
        std::vector<short> parameter_types;
        
        std::set<long> all_offsets;
    
        std::string function_name;
    
        // The line in the file the function starts at
        int start_line;
        
        // The line in the file the function ends at
        int end_line;
        
        // The line of the first real instruction of the function
        int first_instruction_line;
        
        short return_type;
        
        bool filled_symbol_table;
        
        long base_pointer;
        
        
    public:
        FunctionData();
        
        FunctionData(std::string function_name, short return_type);
        
        // Must be in the same order as parameter types
        void add_parameters(std::string parameter, int idx);
        
        void add_variables(std::string variable, short type);
        
        void add_variable_offsets(std::string variable, long offset);
        
        void add_parameter_types(short type);
        
        void add_start_line(int start_line);
        
        void add_end_line(int end_line);
        
        void add_base_pointer(long base_pointer);
        
        void add_first_instruction_line(int first_instruction_line);
        
        long get_base_pointer();
        
        short get_type(const std::string& variable);
        
        long get_offset(const std::string& variable);
        
        long get_variable_address(const std::string& variable);
        
        std::string get_function_name();
        
        int get_start_line();
        
        int get_first_instruction_line();
        
        bool lies_inside_function(int line_number);
        
        // Also checks if it is a parameter
        bool is_variable(const std::string& potential_variable);
        
        bool is_already_added(long offset);
        
        void print_function_data(bool complete_function);
        
        void print_function_name();
        
        void print_return_type();
        
        void print_parameters();
        
        void print_variables();
        
        void print_variable_offsets();
        
        void print_line_info();
        
        void fill_symbol_table(void* base_pointer);
        
        bool has_symbol_table();
        
};

#endif   
