#!/bin/bash

# Script to test the debugger objdump class

UNDERLINE='\e[4m'
END='\e[0m'

# newline for clarity
echo ''

echo 'Should print out all the functions and variables and the corresponding stack offset'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing objdumps parser (C)"
echo 'Testing : ./debugger_objdump -w test_parser.c'
echo ''
./debugger_objdump -w test_parser.c
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing objdumps parser (C++)"
echo 'Testing : ./debugger_objdump -w test_parser.cpp'
echo ''
./debugger_objdump -w test_parser.cpp
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing objdumps parser with no content (C++)"
echo 'Testing : ./debugger_objdump -w test_no_content.cpp'
echo ''
./debugger_objdump -w test_no_content.cpp
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing objdumps parser with no content in a different folder (C++)"
echo 'Testing : ./debugger_objdump -w deep_folder/test_depth.cpp'
echo ''
./debugger_objdump -w deep_folder/test_depth.cpp
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing objdumps parser with content in a different folder (C)"
echo 'Testing : ./debugger_objdump -w ../test_c.c'
echo ''
./debugger_objdump -w ../test_c.c
echo 'Test completed'
echo ''
