/**
 * 	@file: breakpoint_info.h
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */


class BreakpointInfo
{
    private:
        long data;
        
        void* address;
        
        
    public:
        BreakpointInfo();
        
        BreakpointInfo(long data);
        
        BreakpointInfo(long data, void* address);
        
        long get_data();
        
        void* get_address();
        
};
