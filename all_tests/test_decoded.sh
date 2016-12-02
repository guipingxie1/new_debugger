#!/bin/bash

# Script to test the debugger instruction address to line and line to instruction address class

UNDERLINE='\e[4m'
END='\e[0m'

# newline for clarity
echo ''
echo 'Should print out lines to instruction addresses and vice versa'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing decoded lines parsing with input file in different folder (C)"
echo 'Testing : ./debugger_decoded deep_folder/test_function_depth.c'
./debugger_decoded deep_folder/test_function_depth.c 
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing decoded lines parsing with input file in different folder (C++)"
echo 'Testing : ./debugger_decoded deep_folder/test_function_depth.cpp'
./debugger_decoded deep_folder/test_function_depth.cpp
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing decoded lines parsing with input file in same folder (C++)"
echo 'Testing : ./debugger_decoded test_cpp.cpp'
./debugger_decoded test_cpp.cpp
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing decoded lines parsing with bigger file (C)"
echo 'Testing : ./debugger_decoded ../test_c.c'
./debugger_decoded ../test_c.c
echo 'Test completed'
echo ''
