/**
 * 	@file: debugger_test_compile.cpp
 * 	@author: Guiping Xie 
 *
 *  Test file for class CompiledFile
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */
 
#include <iostream> 
 
#include "../compile_file.h" 
#include "../parse_file.h"
#include "../helpers.h"

// g++ -g -std=c++11 debugger_test_compile.cpp -o debugger_compile ../obj/compile_file.o ../obj/helpers.o ../obj/child_helpers.o ../obj/parse_file.o
int main(int argc, char* argv[]) 
{
    CompiledFile cf;
    cf.initialize(argc, argv);
    std::cout << "Successfully compiled child\n";  
    std::string filename = cf.get_file();
    std::cout << "\tFile name : " << filename << "\n";
    cf.remove_compiled_objects();
    
    return 0;
}
