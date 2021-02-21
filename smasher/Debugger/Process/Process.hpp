#pragma once

#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "Debugger/Process/ProcessState.hpp"
#include "Debugger/Process/Mapping/Mapping.hpp"

class Process
{
public:
    static void exec(std::string path, std::vector<std::string> arguments);
public:
    Process(pid_t pid);
    ~Process();
    pid_t getId();
    ProcessState wait();
    Mapping::MappingIterator* getMappings();
private:
    pid_t id;
};