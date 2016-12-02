#!/bin/bash

# Script to test the debugger parse tree class

UNDERLINE='\e[4m'
END='\e[0m'

# newline for clarity
echo ''
echo -e "Diff-ing files - ${UNDERLINE}Should not print anything${END}"
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing gcc tree parser (C)"
echo 'Testing : ./debugger_parse_tree -w test_parser.c > outputs/tester_parser_c.txt'
echo '        : diff outputs/tester_parser_c.txt outputs/function_parse_c.txt'
./debugger_parse_tree -w test_parser.c > outputs/tester_parser_c.txt
diff outputs/tester_parser_c.txt outputs/function_parse_c.txt
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing gcc tree parser with name demangling (C++)"
echo 'Testing : ./debugger_parse_tree -w test_parser.cpp > outputs/tester_parser_cpp.txt'
echo '        : diff outputs/tester_parser_cpp.txt outputs/function_parse_cpp.txt'
./debugger_parse_tree -w test_parser.cpp > outputs/tester_parser_cpp.txt
diff outputs/tester_parser_cpp.txt outputs/function_parse_cpp.txt
echo 'Test completed'
echo ''

echo '-----------------------------------------------------------------------------------------------------'
echo ''
echo -e "Testing gcc tree parser with input file in different folder (C)"
echo 'Testing : ./debugger_parse_tree -w ../test_c.c > outputs/tester_parser_diff_c.txt'
echo '        : diff outputs/tester_parser_diff_c.txt outputs/function_parse_diff_c.txt'
./debugger_parse_tree -w ../test_c.c > outputs/tester_parser_diff_c.txt
diff outputs/tester_parser_diff_c.txt outputs/function_parse_diff_c.txt
echo 'Test completed'
echo ''
