/**
 * 	@file: child_helpers.cpp
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */
 
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <iostream>
 
#include "child_helpers.h"
#include "helpers.h"

void ChildHelper::redirect_pipe_and_exec(const char** exec_arr, int pipe[2], int fd)
{
    // Close read end of pipe for child and redirect to our pipe
	close(pipe[0]);		
    dup2(pipe[1], fd);
    
	execvp(exec_arr[0], (char* const*)exec_arr);
}


void ChildHelper::create_timeout(pid_t child_pid, int pipe[2], const std::string& error_message, const std::string& executable, const std::string& object_file, bool require_exit_status, int exit_status)
{
    // Set timeout child
    // See: http://stackoverflow.com/questions/282176/waitpid-equivalent-with-timeout
    pid_t timeout_pid = fork();
    
    if (timeout_pid == 0)
    {
        usleep(TIMEOUT);
        exit(0);
    }
    
    int status;
    pid_t exited_pid = wait(&status);
    
    // Kill the compile process if it takes too long
    // This can only happen if the pipe becomes full and the child blocks
    if (exited_pid == child_pid)
    {
        kill(timeout_pid, SIGKILL);     
        
        if (require_exit_status)
        {
            kill_if_exit_status(status, exit_status, pipe, executable, object_file);
        }
        
        waitpid(timeout_pid, 0, 0);
    }
    else 
    {
        // Pipe became blocked
        kill(child_pid, SIGKILL);
        
        // Close pipes and exit
        close(pipe[0]);
        close(pipe[1]);
        
        // Remove executable before exiting
        std::cerr << "\n\t\e[1m " << error_message << " \e[0m\n\n";
        Helpers::remove_file(executable);
        Helpers::remove_file(object_file);
        waitpid(child_pid, 0, 0);
        exit(0);
    }
}


void ChildHelper::kill_if_exit_status(int status, int exit_status, int pipe[2], const std::string& executable, const std::string& object_file)
{
    if (WEXITSTATUS(status) == exit_status)
    {
        close(pipe[1]);
        close(pipe[0]);	
        Helpers::remove_file(executable);
        Helpers::remove_file(object_file);
        exit(0);
    }
}
