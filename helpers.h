/**
 * 	@file: helpers.h
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */

#ifndef HELPERS_H 
#define HELPERS_H  
 
#include <string> 
#include <vector>

class Helpers
{
    private:
        static void compute_lps_array(char* pattern, int pattern_length, int* lps);        
        
        
    public:
        static std::string generate_random_string();
    
        static bool object_exists(const std::string& executable);
        
        static void remove_file(const std::string& executable);
 
        static std::vector<int> kmp(char* pattern, char* text);
        
        static std::vector<int> kmp(char* pattern, char* text, int start, int end);
        
        static std::string get_parsed_filename(const std::string& filename);
        
        // Function to help convert types to shorts
        static short get_type(const std::string& type);
        
        // Function to convert type to size of type
        static short get_size(short enum_value);
        
        // Used for printing
        static void print_type_string(short enum_value);
        
        static bool is_number(const std::string& input);
 
 
        // Got these online: http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring

        /**
         *	Trim from start / left (in place)
         *
         * 	@param s: String we want to trim
         */
        static void ltrim(std::string &s);


        /**
         *	Trim from end / right (in place)
         *
         * 	@param s: String we want to trim
         */
        static void rtrim(std::string &s);

        /**
         *	Trim both left and right (in place)
         *
         * 	@param s: String we want to trim
         */
        static void trim(std::string &s);


        /**
         *	Trim from start / left (copying)
         *
         * 	@param s: Copy of string we want to trim
         * 	@return The trimmed string
         */
        static std::string ltrimmed(std::string s);


        /**
         *	Trim from end / right (copying)
         *
         * 	@param s: Copy of string we want to trim
         * 	@return The trimmed string
         */
        static std::string rtrimmed(std::string s);

        /**
         *	Trim both left and right (copying)
         *
         * 	@param s: Copy of string we want to trim
         * 	@return The trimmed string
         */
        static std::string trimmed(std::string s);


        /**
         *	Converts paramter string to lowercase 
         *
         * 	@param str: The command we want to lowercase
         */
        static void str_to_lower( std::string &str );


        /**
         *	Converts paramter string to lowercase 
         *
         * 	@param str: The command we want to lowercase
         */
        static std::string str_to_lowercp( std::string str );
};

#endif
