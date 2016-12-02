#
# 	Makefile for debugger
#

CC = g++
CFLAGS = -std=c++11

# all the source files
SRCS = compile_file.cpp helpers.cpp child_helpers.cpp parse_file.cpp parse_tree.cpp function_data.cpp decoded_line.cpp parse_objdump.cpp tracer.cpp parse_debug_commands.cpp breakpoint_info.cpp

# all the object files
OBJS = $(SRCS:.cpp=.o)

# gets the object files from the correct directory
DIR_OBJS = $(addprefix $(DIR)/, $(OBJS))

# object file directory
DIR = obj


all: debug


debug: $(DIR_OBJS) debugger.cpp
	$(CC) $(CFLAGS) debugger.cpp -o debugger $(DIR_OBJS)


$(DIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@


clean: 
	$(RM) $(DIR)/*.o debugger
