/**
 * 	@file: helpers.cpp
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */

#include <sys/stat.h>
#include <algorithm>
#include <cstring>
#include <iostream>

#include "helpers.h"


//const std::map< std::string, short > Helpers::type_to_short = {{"int", 0}, {"short int", 1}, {"long int", 2}, {"float", 3}, {"double", 4}, {"char", 5}, {"char *", 6}}; 
        
//const std::map< short, short > Helpers::type_to_size = {{0, sizeof(int)}, {1, sizeof(short)}, {2, sizeof(long)}, {3, sizeof(float)}, {4, sizeof(double)}, {5, 1}, {6, -1}};

std::string Helpers::generate_random_string()
{
    char str[10];
    str[0] = '.';
    for (int idx = 1; idx < 9; ++idx)
    {
        int random_int = rand() % 26;
        str[idx] = (char)(random_int + 'a');
    }    
    str[9] = 0;
    
    return std::string(str);
}


short Helpers::get_type(const std::string& type)
{
    if (type.compare("int") == 0)
    {
        return 0;
    }
    
    if (type.compare("short int") == 0)
    {
        return 1;
    }
    
    if (type.compare("long int") == 0)
    {
        return 2;
    }
    
    if (type.compare("float") == 0)
    {
        return 3;
    }
    
    if (type.compare("double") == 0)
    {
        return 4;
    }
    
    if (type.compare("char") == 0)
    {
        return 5;
    }
    
    if (type.compare("char *") == 0)
    {
        return 6;
    }
    
    if (type.compare("void") == 0)
    {
        return 7;
    }
    
    return -1;
}


short Helpers::get_size(short enum_value)
{
    if (enum_value == 0)
    {
        return sizeof(int);
    }
    
    if (enum_value == 1)
    {
        return sizeof(short);
    }
    
    if (enum_value == 2)
    {
        return sizeof(long);
    }
    
    if (enum_value == 3)
    {
        return sizeof(float);
    }
    
    if (enum_value == 4)
    {
        return sizeof(double);
    }
    
    if (enum_value == 5)
    {
        return 1;
    }
    
    if (enum_value == 6)
    {
        return -1;
    }
    
    return 0;
}


void Helpers::print_type_string(short enum_value)
{
    if (enum_value == 0)
    {
        std::cout << "int";
    }
    
    if (enum_value == 1)
    {
        std::cout << "short";
    }
    
    if (enum_value == 2)
    {
        std::cout << "long";
    }
    
    if (enum_value == 3)
    {
        std::cout << "float";
    }
    
    if (enum_value == 4)
    {
        std::cout << "double";
    }
    
    if (enum_value == 5)
    {
        std::cout << "char";
    }
    
    if (enum_value == 6)
    {
        std::cout << "char*";
    }
    
    if (enum_value == 7)
    {
        std::cout << "void";
    }
}


bool Helpers::is_number(const std::string& input)
{
    for (int idx = 0; idx < input.size(); ++idx)
    {
        if (!isdigit(input[idx]))
        {
            return false;
        }
    }
    
    return true;
}


bool Helpers::object_exists(const std::string& executable)
{
    struct stat buffer;   
    return (stat(executable.c_str(), &buffer) == 0); 
}


void Helpers::remove_file(const std::string& executable)
{
    remove(&executable[0]);
}


std::string Helpers::get_parsed_filename(const std::string& filename)
{
    int idx = 0;
    int last_slash = 0;
    
    for (; idx < filename.length(); ++idx)
    {
        if (filename[idx] == '/')
        {
            last_slash = idx + 1;
        }
    }
    
    std::string new_filename(&filename[last_slash]);
    return new_filename;
}


// See: http://www.geeksforgeeks.org/searching-for-patterns-set-2-kmp-algorithm/
std::vector<int> Helpers::kmp(char* pattern, char* text)
{
    std::vector<int> positions;
    int pattern_length = strlen(pattern);
    int text_length = strlen(text);
 
    int lps[pattern_length];
    lps[0] = 0; 
 
    compute_lps_array(pattern, pattern_length, lps);
 
    int i = 0;      // index for text[]
    int j = 0;      // index for pattern[]
    while (i < text_length)
    {
        if (pattern[j] == text[i])
        {
            j++;
            i++;
        }
 
        if (j == pattern_length)
        {
            positions.push_back(i);
            j = lps[j - 1];
        }
 
        else if (i < text_length && pattern[j] != text[i])
        {
            if (j != 0)
            {
                j = lps[j - 1];
            }
            else
            {
                ++i;
            }
        }
    }
    
    return positions;
}


std::vector<int> Helpers::kmp(char* pattern, char* text, int start, int end)
{
    std::vector<int> positions;
    int pattern_length = strlen(pattern);
 
    int lps[pattern_length];
    lps[0] = 0; 
 
    compute_lps_array(pattern, pattern_length, lps);
 
    int i = start;      // index for text[]
    int j = 0;      // index for pattern[]
    while (i < end)
    {
        if (pattern[j] == text[i])
        {
            j++;
            i++;
        }
 
        if (j == pattern_length)
        {
            positions.push_back(i);
            j = lps[j - 1];
        }
 
        else if (i < end && pattern[j] != text[i])
        {
            if (j != 0)
            {
                j = lps[j - 1];
            }
            else
            {
                ++i;
            }
        }
    }
    
    return positions;
}
 
 
// See: http://www.geeksforgeeks.org/searching-for-patterns-set-2-kmp-algorithm/ 
void Helpers::compute_lps_array(char* pattern, int pattern_length, int* lps)
{
    int len = 0;
    int i = 1;
    
    while (i < pattern_length)
    {
        if (pattern[i] == pattern[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else 
        {
            if (len != 0)
            {
                len = lps[len - 1];
            }
            else
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}

 
/**
 *	Trim from start / left (in place)
 */
void Helpers::ltrim(std::string &s) 
{
    s.erase(s.begin(), find_if(s.begin(), s.end(), not1(std::ptr_fun<int, int>(isspace))));
}


/**
 *	Trim from end / right (in place)
 */
void Helpers::rtrim(std::string &s) 
{
    s.erase(find_if(s.rbegin(), s.rend(), not1(std::ptr_fun<int, int>(isspace))).base(), s.end());
}


/**
 *	Trim both left and right (in place)
 */
void Helpers::trim(std::string &s) 
{
    ltrim(s);
    rtrim(s);
}


/**
 *	Trim from start / left (copying)
 */
std::string Helpers::ltrimmed(std::string s) 
{
    ltrim(s);
    return s;
}


/**
 *	Trim from end / right (copying)
 */
std::string Helpers::rtrimmed(std::string s) 
{
    rtrim(s);
    return s;
}


/**
 *	Trim both left and right (copying)
 */
std::string Helpers::trimmed(std::string s) 
{
    trim(s);
    return s;
}


/**
 *	Converts paramter string to lowercase 
 */
void Helpers::str_to_lower(std::string &str) 
{
	transform(str.begin(), str.end(), str.begin(), ::tolower);
}


/**
 *	Converts paramter string to lowercase (copying)
 */
std::string Helpers::str_to_lowercp(std::string str) 
{
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}
