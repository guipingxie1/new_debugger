#!/bin/bash

# Script to test the debugger
#
# Change permissions: chmod +x bash_script_name

# exit if anything fails
set -e

# reset the terminal screen
reset
echo 'Reset screen'
echo ''

BOLD='\e[1m'
UNDERLINE='\e[4m'
YELLOW='\e[1;33m'
RED='\e[1;31m'
GREEN='\e[1;32m'
END='\e[0m'

declare -a all_source_files_array=("debugger_test_compile.cpp" "debugger_test_parse_tree.cpp" "debugger_test_decoded.cpp" "debugger_test_objdump.cpp" "debugger_test_command.cpp")
declare -a all_compiled_array=("debugger_compile" "debugger_parse_tree" "debugger_decoded" "debugger_objdump" "debugger_command")
declare -a all_tests_array=("test_compile.sh" "test_parse_tree.sh" "test_decoded.sh" "test_objdump.sh" "test_command.sh")
declare -a all_object_files_array=("compile_file.o" "helpers.o" "child_helpers.o" "parse_file.o" "decoded_line.o" "parse_objdump.o" "function_data.o" "parse_tree.o" "parse_debug_commands.o")

FLAGS="g++ -std=c++11 -g"
OBJDIR="obj"
ALLOBJS=""

for object in "${all_object_files_array[@]}"
do
    ALLOBJS+=" ../$OBJDIR/$object"
done


for index in "${!all_tests_array[@]}"
do
    ${FLAGS} ${all_source_files_array[$index]} -o ${all_compiled_array[$index]} ${ALLOBJS}
    echo -e "${YELLOW}=======================================================================================================${END}"
    echo ''
    echo -e "${BOLD}Running : ${all_tests_array[$index]}${END}"
    echo ''
    chmod +x ${all_tests_array[$index]}
    ./${all_tests_array[$index]}
    echo -e "${BOLD}Completed : ${all_tests_array[$index]}${END}"
    echo ''
done

