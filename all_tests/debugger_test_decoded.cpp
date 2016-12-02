/**
 * 	@file: debugger_test_decoded.cpp
 * 	@author: Guiping Xie 
 *
 *  Test file for class DecodedLine
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */
 
#include <iostream> 
 
#include "../compile_file.h" 
#include "../parse_file.h"
#include "../decoded_line.h"
#include "../helpers.h"

// g++ -g -std=c++11 debugger_test_decoded.cpp -o debugger_decoded ../obj/compile_file.o ../obj/helpers.o ../obj/child_helpers.o ../obj/parse_file.o ../obj/decoded_line.o
int main(int argc, char* argv[]) 
{
    CompiledFile cf;
    cf.initialize(argc, argv);
    std::string filename = cf.get_file();
    std::string executable = cf.get_compiled_object();
    std::string object_file = cf.get_object_file();
    
    DecodedLine dl;
    dl.initialize(executable, object_file, filename);
    
    dl.print_line_to_address();
    std::cout << "-----------------------\n";
    dl.print_address_to_line();
    
    cf.remove_compiled_objects();
    return 0;
}
