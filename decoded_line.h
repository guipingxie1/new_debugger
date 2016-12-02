/**
 * 	@file: decoded_line.h
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */
 
#ifndef DECODED_H 
#define DECODED_H 

#include <map>
#include <vector>
 
// Instruction address to lines and vice versa 
class DecodedLine
{
    private:
        std::map<int, std::vector<long> > line_number_to_address;
        
        std::map<long, int> address_to_line_number;
        
        void construct_map(const std::string& executable, const std::string& object_file, const std::string& filename);
        
        void parse_objdump_lines(const std::string& filename, char* str);
        
    
    public:
        void initialize(const std::string& executable, const std::string& object_file, const std::string& filename);
        
        // Gets the next valid address of the line number that is no less than the input
        // The second parameter no_match indicates whether the line was directly hit
        // [ 1, 2, 4, 5 ], then 3 would return the address of 4 and set return_line to 4
        // 2 would return the address of 2 and set return_line to 2
        long get_address_from_line(int line_number, int& return_line);
        
        // Returns -1 if the address does not match up with a line number
        int get_line_number_from_address(long address);
        
        // Used for testing - prints the difference between memory addresses
        void print_line_to_address();
        
        // Used for testing - prints the difference between memory addresses
        void print_address_to_line();
};

#endif 
