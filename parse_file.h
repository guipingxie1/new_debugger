/**
 * 	@file: parse_file.h
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */
 
#ifndef PARSE_H 
#define PARSE_H  
 
#include <vector> 
#include <string>
 
class ParsedFile
{
    private:
        // The source file stored as a vector (1 indexed)
		std::vector<std::string> parsed_file;
		
		        
    public:
        void initialize(const std::string& filename);
    
		const std::string get_line(int line_number) const;
		
		void print_line(int line_number) const;
		
		int get_file_length() const; 
};

#endif
