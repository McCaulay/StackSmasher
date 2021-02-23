#pragma once

#include <vector>
#include <map>
#include <string>
#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <sys/reg.h>
#include <sys/user.h>
#include "Debugger/Process/Process.hpp"

class Debugger
{
public:
	static void exec(std::string path, std::vector<std::string> arguments, void (*handler)(Debugger*));
public:
    Debugger(Process* process);
    Process* getProcess();
    void pContinue();
    void pStep();
    void pKill();
    void getRegisters(user_regs_struct* registers);
    uint32_t read(void* address);
    void write(void* address, uint32_t value);    
    void addBreakpoint(void* address);
    void removeBreakpoint(void* address);
private:
    Process* process;
    std::map<void*, uint32_t> breakpoints;
};