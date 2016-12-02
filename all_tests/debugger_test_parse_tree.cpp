/**
 * 	@file: debugger_test_parse_tree.cpp
 * 	@author: Guiping Xie 
 *
 *  Test file for class ParseTree
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */
 
#include <iostream> 
 
#include "../compile_file.h" 
#include "../parse_tree.h"


int main(int argc, char* argv[]) 
{
    CompiledFile cf;
    cf.initialize(argc, argv);
    std::string filename = cf.get_file();
    std::string executable = cf.get_compiled_object();
    std::string object_file = cf.get_object_file();
    
    ParseTree pt;
    pt.initialize(filename, object_file);
    pt.print_all_function_data(false);
    
    cf.remove_compiled_objects();
    return 0;
}
