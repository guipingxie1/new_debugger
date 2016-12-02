/**
 * 	@file: parse_objdump.h
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */
 
#ifndef OBJDUMP_H 
#define OBJDUMP_H

#include "parse_tree.h"
#include "parse_file.h"

class ParsedObjdump
{
    private:
        // Objects which hold useful data
        ParsedFile parsed_file;             // Holds the file contents
        ParseTree parsed_tree;              // Holds the symbol table
    
        // Used to get base pointer
        const char* BASE_POINTER = "(%rbp)\n";
        
        // BAD but lazy
        std::string object_file;        
        std::string executable;
        
        void construct_map(const std::string& filename);
        
        void parse_objdump(const std::string& filename, char* str);
    
        void parse_function(const std::vector<int>& positions, char* str, int start, int end, int function_idx);
        
        int get_line_number(char* str, int index);
        
        void extract_function_info(const std::vector<int>& positions, char* str, int start, int end, int function_idx);
        
        void parse_line(int line_number, char* str, const std::vector<int>& rbp_positions, int function_idx);
        
        void extract_parameters(char* str, int idx, int end_idx, int function_idx);
        
        std::string extract_variable(const std::string& line, int& line_idx, int function_idx);
        
        long extract_offset(char* str, int idx);
        
        bool is_valid_character(char c);
        
        bool is_valid_equal_sign(const std::string& line, int line_idx);
        
    
    public:
        void initialize(const std::string& executable, const std::string& object_file, const std::string& filename);
        
        void print_line(int line_number);
        
        void print_all_function_data();
        
        int get_first_instruction_line(const std::string& function);
        
        void fill_symbol_table(const std::string& function, void* base_pointer);
        
        bool has_symbol_table(const std::string& function);
        
        std::string get_function_from_line(int line_number);
        
        short get_type(const std::string& function, const std::string& variable);
        
        long get_variable_address(const std::string& function, const std::string& variable);
    
};

#endif   
