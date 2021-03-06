#include "Debugger/Debugger.hpp"

void Debugger::exec(std::string path, std::vector<std::string> arguments, void (*handler)(Debugger*))
{
    pid_t child = fork();
    if (child == 0)
    {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        
        // Redirect stdout and stderr to /dev/null
        int devnull = open("/dev/null", O_WRONLY);
        dup2(devnull, 1); // stdout
        dup2(devnull, 2); // stderr
        close(devnull);

        Process::exec(path, arguments);
        exit(0);
        return;
    }

    Log::info(VerbosityLevel::Debug, "Debugger spawned child process: %i\n", child);

    Application::processId = child;
    Process* process = new Process(child);
    Debugger* debugger = new Debugger(process);

    handler(debugger);

    delete process;
    delete debugger;
}

Debugger::Debugger(Process* process)
{
    this->process = process;
}

Process* Debugger::getProcess()
{
    return this->process;
}

void Debugger::pContinue()
{
    ptrace(PTRACE_CONT, this->process->getId(), NULL, NULL);
}

void Debugger::pStep()
{
    ptrace(PTRACE_SINGLESTEP, this->process->getId(), (void*)1, NULL);
}

void Debugger::pKill()
{
    ptrace(PTRACE_KILL, this->process->getId(), NULL, NULL);
}

void Debugger::getRegisters(user_regs_struct* registers)
{
    ptrace(PTRACE_GETREGS, this->process->getId(), NULL, registers);
}

uint32_t Debugger::read(void* address)
{
    return ptrace(PTRACE_PEEKTEXT, this->process->getId(), address, NULL);
}

void Debugger::write(void* address, uint32_t value)
{
    ptrace(PTRACE_POKETEXT, this->process->getId(), address, (void*)value);
}

void Debugger::addBreakpoint(void* address)
{
    uint32_t original = this->read(address);
    this->breakpoints.insert(std::pair<void*, uint32_t>(address, original));
    this->write(address, (original & 0xFFFFFF00) | 0xCC);
}

void Debugger::removeBreakpoint(void* address)
{
    this->write(address, this->breakpoints[address]);
    this->breakpoints.erase(address);
}
