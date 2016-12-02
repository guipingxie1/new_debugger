/**
 * 	@file: decoded_line.cpp
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */
 
#include <unistd.h> 
#include <iostream>
#include <cstring>
#include <algorithm>

#include "decoded_line.h" 
#include "child_helpers.h"
#include "helpers.h"


int DecodedLine::get_line_number_from_address(long address)
{
    if (address_to_line_number.find(address) != address_to_line_number.end())
    {
        return address_to_line_number[address];
    }
    
    return -1;
}


long DecodedLine::get_address_from_line(int line_number, int& return_line)
{
    // Upper bound then check the iterator below it if it is not the beginning
    auto it = line_number_to_address.lower_bound(line_number);
    
    if (it == line_number_to_address.end())
    {
        return 0;
    }
    
    // Set return_line
    return_line = it -> first;
    
    // Return the first address in that line
    return (it -> second)[0];
}


void DecodedLine::initialize(const std::string& executable, const std::string& object_file, const std::string& filename)
{
    construct_map(executable, object_file, filename);
}


void DecodedLine::construct_map(const std::string& executable, const std::string& object_file, const std::string& filename)
{
    int objdump_lines_pipe[2];
    pipe(objdump_lines_pipe);
    
    pid_t objdump_lines = fork();
    
    if (objdump_lines == 0)
    {
        const char* objdump_args[4];
        objdump_args[0] = "objdump";
        objdump_args[1] = "--dwarf=decodedline";
        objdump_args[2] = &executable[0];  
        objdump_args[3] = NULL;
        
        ChildHelper::redirect_pipe_and_exec(objdump_args, objdump_lines_pipe, STDOUT_FILENO);
    }
        
    ChildHelper::create_timeout(objdump_lines, objdump_lines_pipe, "Terminated --- Input File Is Too Big", executable, object_file, false, 0);
    
    close(objdump_lines_pipe[1]);
    
    char str[65536];
    str[0] = 0;
	
	int bytesread = read(objdump_lines_pipe[0], str, sizeof(str));	
	str[bytesread] = 0;
	close(objdump_lines_pipe[0]);
	
	parse_objdump_lines(filename, str);
}


void DecodedLine::parse_objdump_lines(const std::string& filename, char* str)
{
    int new_lines = 0;
    int idx = 0;
    
    std::string parsed_filename = Helpers::get_parsed_filename(filename);
    
    char file[parsed_filename.length() + 1];
    int line;
    long address;
    
    // Useful input is after the 7th line
    while (new_lines != 7)
    {
        if (str[idx++] == '\n')
        {
            ++new_lines;
        }
    }
    
    while (str[idx] != 0)
    {
        if (str[idx] == '\n')
        {
            ++idx;
            
            // End of file
            if (str[idx] == 0)
            {
                break;
            }
        }
        
        sscanf(&str[idx], "%s %d %p\n", file, &line, &address);
        
        file[parsed_filename.length()] = 0;
        
        // End of this program
        if (strcmp(file, &parsed_filename[0]) != 0)
        {
            break;
        }
        
        line_number_to_address[line].push_back(address);
        address_to_line_number[address] = line;
        
        while (str[idx] != 0 && str[idx++] != '\n');
    }
}


void DecodedLine::print_line_to_address()
{
    for (auto it = line_number_to_address.begin(); it != line_number_to_address.end(); ++it)
    {
        int line_number = it -> first;
        std::vector<long> addresses = it -> second;
        for (int idx = 0; idx < addresses.size(); ++idx) 
        {
            std::cout << line_number << "\t" << (void*)addresses[idx] << "\n";
        }
    }
}


void DecodedLine::print_address_to_line()
{
    for (auto it = address_to_line_number.begin(); it != address_to_line_number.end(); ++it)
    {
        long address = it -> first;
        int line_number = it -> second;
        std::cout << (void*)address << "\t" << line_number << "\n";
    }
}
