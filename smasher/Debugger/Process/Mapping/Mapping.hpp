/*
 @Author	: ouadimjamal@gmail.com
 @date		: December 2015
 @see       : https://github.com/ouadev/proc_maps_parser

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.  No representations are made about the suitability of this
software for any purpose.  It is provided "as is" without express or
implied warranty.
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/limits.h>
#include <stdint.h>
#include <vector>
#include <string>
#include "Log/Log.hpp"

// Maximum line length in a procmaps file
#define PROCMAPS_LINE_MAX_LENGTH  (PATH_MAX + 100) 

class Mapping
{
public:
    /**
     * Holds iterating information
     */
    struct MappingIterator
    {
        Mapping* head;
        Mapping* current;
    };
public:
    /**
     * Parse the mapping for the process id.
     * 
     * @param pid the process id whose memory map to be parser. the current process if pid<0
     * @return an iterator over all the nodes
     */
    static Mapping::MappingIterator* parse(int pid);

    /**
     * Move between areas
     * @param iterator The iterator to move on step in the chained list
     * @return A procmaps structure filled with information about this VM area
     */
    static Mapping* next(Mapping::MappingIterator* iterator);
    /**
     * Should be called at the end to free the resources
     * @param iterator The iterator structure returned by pmparser_parse
     */
    static void freeAll(Mapping::MappingIterator* iterator);

    /**
     * @param map The head of the list
     * @order The order of the area to print, -1 to print everything
     */
    static void print(Mapping* map, int order);

private:
    /**
     * Internal usage.
     */
    static void splitLine(char* buf, char* addr1, char* addr2, char* perm, char* offset, char* device, char* inode, char* pathname);

public:
    uint8_t* read(void* address, size_t length);
    void write(void* address, uint8_t* buffer, size_t length);
    void* search(std::vector<uint8_t> pattern);

    pid_t processId;

    void* addr_start;                 // Start address of the area
    void* addr_end;                   // End address
    unsigned long length;             // Size of the range

    char perm[5];                     // Rermissions rwxp
    short is_r;
    short is_w;
    short is_x;
    short is_p;

    long offset;                      // Offset
    char dev[12];                     // Dev major:minor
    int inode;                        // Inode of the file that backs the area

    char pathname[600];               // The path of the file that backs the area
    Mapping* nextItem;                // Handler of the chained list
private:
    std::string getMemoryFile();
};
