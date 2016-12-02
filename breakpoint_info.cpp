/**
 * 	@file: breakpoint_info.cpp
 * 	@author: Guiping Xie 
 *
 * 	@description: UIUC - CS242 (Programming Studio) Final Project 
 *	@date: Fall 2016
 *
 */

#include "breakpoint_info.h"

 
BreakpointInfo::BreakpointInfo(long data, void* address)
{
    this -> data = data;
    this -> address = address;
}


BreakpointInfo::BreakpointInfo(long data)
{
    this -> data = data;
}


BreakpointInfo::BreakpointInfo()
{
    // Nothing
}


long BreakpointInfo::get_data()
{
    return data;
}


void* BreakpointInfo::get_address()
{
    return address;
}
