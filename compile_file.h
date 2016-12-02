/**
 * 	@file: compile_file.h
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */

#ifndef COMPILE_H 
#define COMPILE_H 

#include <string>

class CompiledFile 
{
    private:
        static const int ERROR_STATUS = 42;
        
        bool compile_without_warnings;
    
        std::string filename;
    
        std::string compiled_object;
        
        std::string object_file;
        
        void set_filename(int argc, char* argv[]);
        
        bool is_valid_file();
        
        void compile_file();
        
        void compile_object_file();
        
        void show_errors();
        
        void execute_command(bool has_pipe, int compile_error_pipe[2], bool create_object_file);
        
    
    public:
        std::string get_compiled_object();
        
        std::string get_object_file(); 
        
        std::string get_file();
        
        void remove_compiled_objects();       
        
        void initialize(int argc, char* argv[]);
    
};


#endif
