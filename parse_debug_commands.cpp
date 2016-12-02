/**
 * 	@file: parse_debug_commands.cpp
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */

#include <fstream>
#include <iostream> 

#include "parse_debug_commands.h"
#include "helpers.h"

// Map to map the commands to numbers so we do not have to store the strings
static std::map<std::string, int> command_to_int_encoding =
{
    {"RUN", 1},
    {"CONTINUE", 2},
    {"STEP", 3},
    {"BREAK", 4},
    {"WATCH", 5},
    {"REMOVE_BREAKPOINT", 6},
    {"REMOVE_WATCHPOINT", 7},
    {"POINT_INFO", 8},
    {"QUIT", 9},
    {"PRINT", 10},
    {"HELP", 11}
};


void ParsedCommands::initialize(const std::string& executable, const std::string& object_file)
{
    // From http://www.cplusplus.com/reference/fstream/ifstream/ifstream/
    std::ifstream debug_commands("debugger_input.txt", std::ifstream::in);
    check_if_valid_file(executable, object_file, debug_commands, "debugger_input.txt");    
    parse_file_for_commands(debug_commands, executable, object_file);
}


ParsedCommands::ParsedCommands()
{
    // Nothing
}


ParsedCommands::ParsedCommands(const std::string& executable, const std::string& object_file, const char* command_file)
{
    std::ifstream debug_commands(command_file, std::ifstream::in);
    check_if_valid_file(executable, object_file, debug_commands, command_file);    
    parse_file_for_commands(debug_commands, executable, object_file);
}


int ParsedCommands::get_debug_command(const std::string& command)
{
    auto it = all_debug_commands.find(command);
    
    if (it == all_debug_commands.end())
    {
        return -1;
    }
    
    return it -> second;
}   


bool ParsedCommands::check_if_valid_file(const std::string& executable, const std::string& object_file, const std::ifstream& debug_commands, const char* command_file)
{
    if (!debug_commands.is_open())
    {
        std::cerr << "\n\t\e[1m Cannot find file : " << command_file << " \e[0m\n\n";
        Helpers::remove_file(executable);
        Helpers::remove_file(object_file);
        exit(0);
    }
}


void ParsedCommands::parse_file_for_commands(std::ifstream& debug_commands, const std::string& executable, const std::string& object_file)
{
    std::string line;
    while (getline(debug_commands, line)) 
    {
	    get_commands(line, executable, object_file);
    }
    
    debug_commands.close();
}


void ParsedCommands::get_commands(const std::string& line, const std::string& executable, const std::string& object_file)
{
    int idx = 0;
    int command_type = get_command_type(idx, line, executable, object_file);
    
    int len = line.length();
    int end_idx = idx;
    while (end_idx < len)
    {
        while (end_idx < len && line[end_idx] != ',')
        {
            ++end_idx;
        }                
                
        std::string command = line.substr(idx + 1, end_idx - idx - 1);
        Helpers::trim(command);
        
        auto it = all_debug_commands.find(command);
        
        // Check that the same command is not mapped to two different debug commands
        if (it != all_debug_commands.end())
        {
            // If it is mapped to a different debug command already
            if (it -> second != command_type)
            {
                std::cerr << "\n\t\e[1m Command : " << command << " is already used \e[0m\n\n";
                Helpers::remove_file(executable);
                Helpers::remove_file(object_file);
                exit(0);
            }
        }
        
        all_debug_commands[command] = command_type;
        idx = end_idx;
        ++end_idx;
    }
}


int ParsedCommands::get_command_type(int& idx, const std::string& line, const std::string& executable, const std::string& object_file)
{
    int len = line.length();
    
    while (idx != len && line[idx] != '=')
    {
        ++idx;
    } 
    
    std::string type = line.substr(0, idx);
    
    auto it = command_to_int_encoding.find(type);
    
    if (it != command_to_int_encoding.end())
    {
        return it -> second;
    }
    
    std::cerr << "\n\t\e[1m Debug command : " << type << " is not an accepted debug command \e[0m\n\n";
    Helpers::remove_file(executable);
    Helpers::remove_file(object_file);
    exit(0);
}


void ParsedCommands::print_all_commands()
{
    std::vector<std::string> debug_commands[12];
    for (auto it = all_debug_commands.begin(); it != all_debug_commands.end(); ++it)
    {
        int command_enum = it -> second;
        std::string command = it -> first;        
        debug_commands[command_enum].push_back(command);
    }
    
    auto it = command_to_int_encoding.begin();
    for (int idx = 1; idx < 12; ++idx)
    {
        std::cout << "The command(s) for : " << it -> first << " is/are\n";
        
        std::vector<std::string> commands = debug_commands[idx];
        for (int command_idx = 0; command_idx < commands.size(); ++command_idx)
        {
            std::cout << "\t" << commands[command_idx] << "\n";
        }
        
        ++it;
    }
}
