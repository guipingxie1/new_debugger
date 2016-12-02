/**
 * 	@file: parse_file.cpp
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */

#include <fstream>
#include <iostream>

#include "parse_file.h"
#include "helpers.h"


void ParsedFile::initialize(const std::string& filename)
{
    // Might be possible that we cannot open the file
    std::ifstream source(filename);	
	std::string line;
	
	// make vector 1 indexed
	parsed_file.push_back("");				

	while (getline(source, line)) 
	{
		Helpers::trim(line);
		parsed_file.push_back(line);
	}

	source.close();
}


const std::string ParsedFile::get_line(int line_number) const
{
    if (line_number < parsed_file.size() && line_number >= 0)
    {
        return parsed_file[line_number];
    }
    else
    {
        // Warning here!!!
        // Should never hit this part regardless
        return std::string();
    }
}


void ParsedFile::print_line(int line_number) const
{
    if (line_number < parsed_file.size() && line_number >= 0)
    {
        std::cout << parsed_file[line_number] << "\n";
    }
}
	
		
int ParsedFile::get_file_length() const
{
    return parsed_file.size();
}
