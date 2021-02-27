#pragma once

#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "Stages/Application.hpp"
#include "Debugger/Process/ProcessState.hpp"
#include "Debugger/Process/Mapping/Mapping.hpp"

class Process
{
public:
    static void exec(std::string path, std::vector<std::string> arguments);
    static std::string execWithOutput(std::string path, std::vector<std::string> arguments);
private:
    static std::vector<char*> getArguments(std::string app, std::vector<std::string> arguments);
public:
    Process(pid_t pid);
    ~Process();
    pid_t getId();
    ProcessState wait();
    MappingIterator* getMappings();
    uint8_t* read(void* address, size_t length);
    void write(void* address, uint8_t* buffer, size_t length);
    void* search(std::vector<uint8_t> pattern);
private:
    pid_t id;
};