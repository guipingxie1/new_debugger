/**
 * 	@file: parse_tree.h
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */

#ifndef PARSETREE_H 
#define PARSETREE_H 

#include "function_data.h"

class ParseTree
{
    private:
        std::vector< FunctionData > function_data;
        
        std::map< std::string, int > function_to_idx;
    
        std::string temporary_executable;
        
        void generate_parse_tree(const std::string& filename, const std::string& object_file);
        
        void parse_the_tree(char* str, int len);
        
        void parse_function(int idx, char* str, int stop_idx);
        
        void fill_parameter_data(int idx, char* str, FunctionData& fd);
        
        void fill_variable_data(int idx, char* str, int stop_idx, FunctionData& fd);
        
        void recursive_fill_data(int& idx, char* str, int stop_idx, FunctionData& fd);
        
        void parse_line_for_variables(int idx, int end_idx, char* str, FunctionData& fd);
        
        bool found_end_idx(char* str, int& end_idx, int stop_idx);
        
        bool found_new_bracket(char* str, int& end_idx, int stop_idx);
        
        bool is_function(const std::string& potential_function);
        
    
    public:
        void initialize(const std::string& filename, const std::string& object_file);
        
        FunctionData& get_function(int function_idx);
        
        //FunctionData& get_function(std::string function);
        
        void print_all_function_data(bool print_everything);        
        
        // Careful - No bound checking
        void add_start_line(int function_idx, int start_line);
        
        void add_end_line(int function_idx, int end_line);
        
        void add_first_instruction_line(int function_idx, int first_instruction_line);
        
        bool is_already_added(int function_idx, long offset);
        
        void add_variable_offsets(int function_idx, const std::string& variable, long offset);
        
        int get_start_line(int function_idx);        
        
        int get_first_instruction_line(int function_idx);
        
        int get_start_line(const std::string& function);
        
        int get_first_instruction_line(const std::string& function);
        
        std::string get_function_name(int function_idx);
        
        void fill_symbol_table(const std::string& function, void* base_pointer);
        
        bool has_symbol_table(const std::string& function);
        
        void add_parameters(int function_idx, const std::string& parameter, int count);
        
        bool is_variable(int function_idx, const std::string& variable);
        
        std::string get_function_from_line(int line_number);
        
        short get_type(const std::string& function, const std::string& variable);
        
        long get_variable_address(const std::string& function, const std::string& variable);
        
};


#endif
