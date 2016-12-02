#!/bin/bash

# Script to test the debugger compiler class

RED='\e[1;31m'
GREEN='\e[1;32m'
END='\e[0m'

# newline for clarity
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing compiling with warnings enabled (Should give ${RED}error${END} and exit without printing success)"
echo 'Testing : ./debugger_compile test_warnings.c'
./debugger_compile test_warnings.c
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing compiling with warnings disabled (Should ${GREEN}pass${END} through)"
echo 'Testing : ./debugger_compile test_warnings.c -w'
./debugger_compile test_warnings.c -w
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing compiling with warnings disabled (Switched order of parameters - Should ${GREEN}pass${END} through)"
echo 'Testing : ./debugger_compile -w test_warnings.c'
./debugger_compile -w test_warnings.c
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing compiling with warnings disabled (Added extra parameters - Should ${GREEN}pass${END} through)"
echo 'Testing : ./debugger_compile -w test_warnings.c random1 random2'
./debugger_compile -w test_warnings.c random1 random2
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing compiling error file (Should give ${RED}error${END} and exit without printing success)"
echo 'Testing : ./debugger_compile test_errors.c'
./debugger_compile test_errors.c
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing file does not exist (Should give ${RED}error${END} and exit without printing success)"
echo 'Testing : ./debugger_compile invalid_file.c'
./debugger_compile invalid_file.c
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing empty input with the w flag (Should give ${RED}error${END} and exit without printing success)"
echo 'Testing : ./debugger_compile -w'
./debugger_compile -w
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing empty input (Should give ${RED}error${END} and exit without printing success)"
echo 'Testing : ./debugger_compile'
./debugger_compile
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing C++ program (Should ${GREEN}pass${END} through)"
echo 'Testing : ./debugger_compile test_cpp.cpp'
./debugger_compile test_cpp.cpp
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing input file in different folder (C - Should ${GREEN}pass${END} through)"
echo 'Testing : ./debugger_compile deep_folder/test_depth.c'
./debugger_compile deep_folder/test_depth.c
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing input file in different folder (C++ - Should ${GREEN}pass${END} through)"
echo 'Testing : ./debugger_compile deep_folder/test_depth.cpp'
./debugger_compile deep_folder/test_depth.cpp
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing compile error in different folder (Should give ${RED}error${END} and exit without printing success)"
echo 'Testing : ./debugger_compile deep_folder/compile_error.c'
./debugger_compile deep_folder/compile_error.c
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing non program file (Should give ${RED}error${END} and exit without printing success)"
echo 'Testing : ./debugger_compile not_a_program.txt'
./debugger_compile not_a_program.txt
echo 'Test completed'
echo ''
