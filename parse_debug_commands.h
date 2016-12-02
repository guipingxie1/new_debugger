/**
 * 	@file: parse_debug_commands.h
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */
 
#ifndef PARSECOMMAND_H 
#define PARSECOMMAND_H 

#include <map>
#include <string>

class ParsedCommands
{
    private:        
        std::map<std::string, int> all_debug_commands;
        
        bool check_if_valid_file(const std::string& executable, const std::string& object_file, const std::ifstream& debug_commands, const char* command_file);
        
        void parse_file_for_commands(std::ifstream& debug_commands, const std::string& executable, const std::string& object_file);
        
        void get_commands(const std::string& line, const std::string& executable, const std::string& object_file);
        
        int get_command_type(int& idx, const std::string& line, const std::string& executable, const std::string& object_file);
        
    
    public:
        ParsedCommands();
    
        ParsedCommands(const std::string& executable, const std::string& object_file, const char* command_file);
        
        void initialize(const std::string& executable, const std::string& object_file);
        
        int get_debug_command(const std::string& command);
        
        void print_all_commands();
        
        
};


#endif 
