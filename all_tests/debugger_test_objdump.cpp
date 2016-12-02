/**
 * 	@file: debugger_test_objdump.cpp
 * 	@author: Guiping Xie 
 *
 *  Test file for class ParseObjdump
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */
 
#include <iostream> 
 
#include "../compile_file.h" 
#include "../parse_file.h"
#include "../parse_objdump.h"
#include "../parse_tree.h"
#include "../helpers.h"

// g++ -g -std=c++11 debugger_test_objdump.cpp -o debugger_objdump ../obj/compile_file.o ../obj/helpers.o ../obj/child_helpers.o ../obj/parse_file.o ../obj/decoded_line.o ../obj/parse_objdump.o
int main(int argc, char* argv[]) 
{
    CompiledFile cf;
    cf.initialize(argc, argv);
    std::string filename = cf.get_file();
    std::string executable = cf.get_compiled_object();
    std::string object_file = cf.get_object_file();
    
    ParsedObjdump po;
    po.initialize(executable, object_file, filename);
    po.print_all_function_data();
    
    cf.remove_compiled_objects();
    return 0;
}
