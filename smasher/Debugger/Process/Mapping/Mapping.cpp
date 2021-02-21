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

#include "Debugger/Process/Mapping/Mapping.hpp"

Mapping::MappingIterator* Mapping::parse(int pid)
{
    Mapping::MappingIterator* maps_it = (Mapping::MappingIterator*)malloc(sizeof(Mapping::MappingIterator));
    char filepath[500];
    if (pid >= 0)
        sprintf(filepath,"/proc/%d/maps", pid);
    else
        sprintf(filepath,"/proc/self/maps");

    FILE* file = fopen(filepath, "r");
    if (!file)
        return nullptr;

    int ind = 0;
    char buf[PROCMAPS_LINE_MAX_LENGTH];
    int c;
    Mapping* mapList = nullptr;
    Mapping* tmp;
    Mapping* currentNode = mapList;
    char addr1[20], addr2[20], perm[8], offset[20], dev[10], inode[30], pathname[PATH_MAX];
    while(!feof(file))
    {
        fgets(buf,PROCMAPS_LINE_MAX_LENGTH,file);

        // Allocate a node
        tmp = (Mapping*)malloc(sizeof(Mapping));

        // Set the process id
        tmp->processId = pid;

        // Fill the node
        Mapping::splitLine(buf, addr1, addr2, perm, offset, dev, inode, pathname);

        // addr_start & addr_end
        unsigned long l_addr_start;
        sscanf(addr1,"%lx", (long unsigned *)&tmp->addr_start);
        sscanf(addr2,"%lx", (long unsigned *)&tmp->addr_end);
        
        // Size
        tmp->length = (unsigned long)((char*)tmp->addr_end - (char*)tmp->addr_start);
        
        // Perm
        strcpy(tmp->perm, perm);
        tmp->is_r = (perm[0] == 'r');
        tmp->is_w = (perm[1] == 'w');
        tmp->is_x = (perm[2] == 'x');
        tmp->is_p = (perm[3] == 'p');

        // Offset
        sscanf(offset, "%lx", &tmp->offset);
        
        // Device
        strcpy(tmp->dev, dev);
        
        // Inode
        tmp->inode = atoi(inode);
        
        // Pathname
        strcpy(tmp->pathname, pathname);
        tmp->nextItem = nullptr;
        
        // Attach the node
        if (ind==0)
        {
            mapList = tmp;
            mapList->nextItem = nullptr;
            currentNode = mapList;
        }
        currentNode->nextItem = tmp;
        currentNode = tmp;
        ind++;
    }

    // Close file
    fclose(file);

    maps_it->head = mapList;
    maps_it->current = mapList;
    return maps_it;
}

Mapping* Mapping::next(Mapping::MappingIterator* iterator)
{
    if (iterator->current == NULL)
        return NULL;
    Mapping* p_current = iterator->current;
    iterator->current = iterator->current->nextItem;
    return p_current;
}

void Mapping::freeAll(Mapping::MappingIterator* iterator)
{
    Mapping* maps_list = iterator->head;
    if (maps_list == NULL)
        return;
    Mapping* act=maps_list;
    Mapping* nxt=act->nextItem;
    while (act != NULL)
    {
        free(act);
        act=nxt;
        if (nxt != NULL)
            nxt = nxt->nextItem;
    }
}

void Mapping::splitLine(char* buf, char* addr1, char* addr2, char* perm, char* offset, char* device, char* inode, char* pathname)
{
    int orig=0;
    int i=0;
    
    // addr1
    while (buf[i]!='-')
    {
        addr1[i-orig]=buf[i];
        i++;
    }
    addr1[i]='\0';
    i++;
    
    // addr2
    orig=i;
    while(buf[i]!='\t' && buf[i]!=' ')
    {
        addr2[i-orig]=buf[i];
        i++;
    }
    addr2[i-orig]='\0';

    // perm
    while(buf[i]=='\t' || buf[i]==' ')
        i++;
    orig=i;
    while(buf[i]!='\t' && buf[i]!=' ')
    {
        perm[i-orig]=buf[i];
        i++;
    }
    perm[i-orig]='\0';
    
    // offset
    while(buf[i]=='\t' || buf[i]==' ')
        i++;
    orig=i;
    while(buf[i]!='\t' && buf[i]!=' ')
    {
        offset[i-orig]=buf[i];
        i++;
    }
    offset[i-orig]='\0';
    
    // dev
    while(buf[i]=='\t' || buf[i]==' ')
        i++;
    orig=i;
    while(buf[i]!='\t' && buf[i]!=' ')
    {
        device[i-orig]=buf[i];
        i++;
    }
    device[i-orig]='\0';
    
    // inode
    while(buf[i]=='\t' || buf[i]==' ')
        i++;
    orig=i;
    while(buf[i]!='\t' && buf[i]!=' ')
    {
        inode[i-orig]=buf[i];
        i++;
    }
    inode[i-orig]='\0';
    
    // pathname
    pathname[0]='\0';
    while(buf[i]=='\t' || buf[i]==' ')
        i++;
    orig=i;
    while(buf[i]!='\t' && buf[i]!=' ' && buf[i]!='\n')
    {
        pathname[i-orig]=buf[i];
        i++;
    }
    pathname[i-orig]='\0';
}

std::string Mapping::getMemoryFile()
{
    return "/proc/" + (this->processId >= 0 ? std::to_string(this->processId) : "self") + "/mem";
}

uint8_t* Mapping::read(void* address, size_t length)
{
    int memory = open(this->getMemoryFile().c_str(), O_RDONLY);
    if (!memory)
        return nullptr;

    void* buffer = malloc(length);
    pread(memory, buffer, length, (off_t)address);
    close(memory);
    return (uint8_t*)buffer;
}

void Mapping::write(void* address, uint8_t* buffer, size_t length)
{
    int memory = open(this->getMemoryFile().c_str(), O_RDONLY);
    if (!memory)
        return;

    pwrite(memory, buffer, length, (off_t)address);
    close(memory);
}

void* Mapping::search(std::vector<uint8_t> pattern)
{
    for (uint32_t address = (uint32_t)this->addr_start; address < (uint32_t)this->addr_end; address += 0x1000)
    {
        uint8_t* buffer = this->read((void*)address, 0x1000);
        for (uint32_t j = 0; j < 0x1000; j++)
        {
            bool matches = true;
            for (uint32_t k = 0; k < pattern.size(); k++)
            {
                if (buffer[j + k] != pattern[k])
                {
                    matches = false;
                    break;
                }
            }

            if (matches)
            {
                free(buffer);
                buffer = nullptr;
                return (void*)(address + j);
            }
        }

        free(buffer);
        buffer = nullptr;
    }
    return nullptr;
}
