/**
 * 	@file: compile_file.cpp
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <cstring>
#include <iostream>

#include "compile_file.h"
#include "helpers.h"
#include "child_helpers.h"


std::string CompiledFile::get_compiled_object() 
{
    return compiled_object;
}


std::string CompiledFile::get_object_file() 
{
    return object_file;
}


std::string CompiledFile::get_file() 
{
    return filename;
}


void CompiledFile::remove_compiled_objects()
{  
    remove(&compiled_object[0]);
    remove(&object_file[0]);
}


void CompiledFile::set_filename(int argc, char* argv[])
{
    int filename_idx = 1;
    bool got_file = false;
    for (int idx = 1; idx < argc; ++idx) 
    {
        if (argv[idx][0] != '-' && !got_file) 
        {
            filename_idx = idx;
            got_file = true;
        }
        
        if (strlen(argv[idx]) == 2 && argv[idx][0] == '-' && argv[idx][1] == 'w')
        {
            compile_without_warnings = true;
        }
    } 
    
    if (!got_file)
    {
        std::cerr << "\n\t\e[1m No Input File Detected \e[0m\n\n";
        exit(0);
    }
    else if (!Helpers::object_exists(argv[filename_idx]))
    {
        std::cerr << "\n\t\e[1m Cannot Located File: " << argv[filename_idx] << " \e[0m\n\n";
        exit(0);
    }
    
    filename = argv[filename_idx];
}


void CompiledFile::initialize(int argc, char* argv[])
{
    compile_without_warnings = false;
    set_filename(argc, argv);
    
    // Random seeding to get better random numbers
    srand(time(NULL));
    
    compiled_object = Helpers::generate_random_string();
    compile_file();
    object_file = Helpers::generate_random_string();
    compile_object_file();
}


void CompiledFile::compile_object_file()
{
    pid_t object_pid = fork();
    
    if (object_pid == 0) 
    {
        int ignore[2];
    	execute_command(false, ignore, true);    
    }
    
    waitpid(object_pid, 0, 0);
}


void CompiledFile::compile_file()
{
    // Pipe to check if there are compile errors / warnings
    int compile_error_pipe[2];
    pipe(compile_error_pipe);
    
    pid_t compile_file = fork();
    
    if (compile_file == 0)
    {
        execute_command(true, compile_error_pipe, false);
    }

    ChildHelper::create_timeout(compile_file, compile_error_pipe, "Terminated --- Too Many Compile Errors/Warnings", compiled_object, "__ignore", true, ERROR_STATUS);
    
    // Parent does not write to pipe
    close(compile_error_pipe[1]);
    
    // Initiate reading from the pipe to check if there are compile errors
    char str[2];
    str[0] = 0;
	
	int bytesread = read(compile_error_pipe[0], str, sizeof(str));	
	str[1] = 0;
	close(compile_error_pipe[0]);
	
	// If there are compiler errors
	if (str[0] != 0) 
	{
	    show_errors();
	    exit(0);
    }
}


void CompiledFile::show_errors()
{
    std::cerr << "\n\t\e[1m Compile Errors/Warnings Detected \e[0m\n\n";

    pid_t compile_error = fork();
    
    if (compile_error == 0) 
    {
        int ignore[2];
    	execute_command(false, ignore, false);    
    }
    
    waitpid(compile_error, 0, 0);
    remove_compiled_objects();
}


bool CompiledFile::is_valid_file()
{
    int length = filename.length();
    
    if (length >= 3 && strcmp(&filename[length - 2], ".c") == 0)
    {
        return true;
    }
    
    if (length >= 5 && strcmp(&filename[length - 4], ".cpp") == 0)
    {
        return true;
    } 
    
    std::cout << "\n\t\e[1m Invalid Input File \e[0m\n\n";
    exit(ERROR_STATUS);    
}


void CompiledFile::execute_command(bool has_pipe, int compile_error_pipe[2], bool create_object_file)
{
    const char* compile_args[9];
    compile_args[6] = NULL;
    compile_args[7] = NULL;
    compile_args[8] = NULL;
    
    if (is_valid_file())
    {
        compile_args[0] = "g++";
        compile_args[1] = "-std=c++11";
    }
    
    compile_args[2] = "-g";
    compile_args[3] = &filename[0];
    compile_args[4] = "-o";
    
    // Test here
    if (create_object_file)
    {
        compile_args[5] = &object_file[0];
        
        if (compile_without_warnings)
        {
            compile_args[6] = "-w";
            compile_args[7] = "-c";
        }
        else
        {
            compile_args[6] = "-c";
        }
    }
    else 
    {
        compile_args[5] = &compiled_object[0];
        
        if (compile_without_warnings)
        {
            compile_args[6] = "-w";
        }
    }
    
    if (has_pipe)
    {
        ChildHelper::redirect_pipe_and_exec(compile_args, compile_error_pipe, STDERR_FILENO);
    } 
    else
    {
        execvp(compile_args[0], (char* const*)compile_args);
    }
}
