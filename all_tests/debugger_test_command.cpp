/**
 * 	@file: debugger_test_command.cpp
 * 	@author: Guiping Xie 
 *
 *  Test file for class ParsedCommands
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */
 
#include <iostream> 
 
#include "../parse_debug_commands.h" 

// g++ -g -std=c++11 debugger_test_command.cpp -o debugger_command ../obj/parse_debug_commands.o ../obj/helpers.o
int main(int argc, char* argv[]) 
{
    ParsedCommands pc("__invalid", "__invalid", argv[1]);
    pc.print_all_commands();
    
    return 0;
}
