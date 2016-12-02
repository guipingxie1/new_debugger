/**
 * 	@file: child_helpers.h
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */
 
#ifndef CHILDHELP_H 
#define CHILDHELP_H 

#include <string>


class ChildHelper
{
    private:
        // 2.5 second timeout
        static const int TIMEOUT = 2500000;
        
        static void kill_if_exit_status(int status, int exit_status, int pipe[2], const std::string& executable, const std::string& object_file);
        

    public:
        /**
         *  Creates a timeout process which will kill the passed-in child process if it takes too long
         *
         *  param: child_pid - the passed in child process, will be killed if it takes longer than the timeout process (see TIMEOUT above)
         */
        static void create_timeout(pid_t child_pid, int pipe[2], const std::string& error_message, const std::string& executable, const std::string& object_file, bool require_exit_status, int exit_status);
    
        static void redirect_pipe_and_exec(const char** exec_arr, int pipe[2], int fd);
        
};

#endif
