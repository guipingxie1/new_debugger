#!/bin/bash

# Script to test the debugger command class

RED='\e[1;31m'
GREEN='\e[1;32m'
END='\e[0m'

# newline for clarity
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing parsing debug commands in same folder (${GREEN}Pass${END} without printing)"
echo 'Testing : ./debugger_command command_input.txt > outputs/tester_commands_same_folder.txt'
echo '          diff outputs/tester_commands_same_folder.txt outputs/parsed_commands_same_folder.txt'
./debugger_command command_input.txt > outputs/tester_commands_same_folder.txt
diff outputs/tester_commands_same_folder.txt outputs/parsed_commands_same_folder.txt
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing parsing debug commands in different folder (${GREEN}Pass${END} without printing)"
echo 'Testing : ./debugger_command ../debugger_input.txt > outputs/tester_commands_diff_folder.txt'
echo '          diff outputs/tester_commands_diff_folder.txt outputs/parsed_commands_diff_folder.txt'
./debugger_command ../debugger_input.txt > outputs/tester_commands_diff_folder.txt
diff outputs/tester_commands_diff_folder.txt outputs/parsed_commands_diff_folder.txt
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing parsing debug commands out of order (${GREEN}Pass${END} without printing)"
echo 'Testing : ./debugger_command command_input_out_order.txt > outputs/tester_commands_out_order.txt'
echo '          diff outputs/tester_commands_out_order.txt outputs/parsed_commands_out_order.txt'
./debugger_command command_input_out_order.txt > outputs/tester_commands_out_order.txt
diff outputs/tester_commands_out_order.txt outputs/parsed_commands_out_order.txt
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing parsing debug commands with random spaces (${GREEN}Pass${END} without printing)"
echo 'Testing : ./debugger_command command_input_spaces.txt > outputs/tester_commands_spaces.txt'
echo '          diff outputs/tester_commands_spaces.txt outputs/parsed_commands_spaces.txt'
./debugger_command command_input_spaces.txt > outputs/tester_commands_spaces.txt
diff outputs/tester_commands_spaces.txt outputs/parsed_commands_spaces.txt
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing parsing debug commands with repeated commands (for same command --- Should be ${GREEN}okay${END})"
echo 'Testing : ./debugger_command command_input_good_repeated.txt > outputs/tester_commands_good_repeated.txt'
echo '          diff outputs/tester_commands_good_repeated.txt outputs/parsed_commands_repeated.txt'
./debugger_command command_input_good_repeated.txt > outputs/tester_commands_good_repeated.txt
diff outputs/tester_commands_good_repeated.txt outputs/parsed_commands_repeated.txt
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing parsing debug commands with repeated RUN command (Should be ${GREEN}okay${END})"
echo 'Testing : ./debugger_command command_input_repeated_run.txt > outputs/tester_commands_run_repeated.txt'
echo '          diff outputs/tester_commands_run_repeated.txt outputs/parsed_commands_run_repeated.txt'
./debugger_command command_input_repeated_run.txt > outputs/tester_commands_run_repeated.txt
diff outputs/tester_commands_run_repeated.txt outputs/parsed_commands_run_repeated.txt
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing parsing debug commands with repeated commands (for different command --- ${RED}Error${END})"
echo 'Testing : ./debugger_command command_input_bad_repeated.txt'
./debugger_command command_input_bad_repeated.txt
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing non acceptable debug command --- ${RED}Error${END}"
echo 'Testing : ./debugger_command command_invalid_input.txt'
./debugger_command command_invalid_input.txt
echo 'Test completed'
echo ''
