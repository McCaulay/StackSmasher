#include "Stages/Fuzzer/Fuzzer.hpp"

int Fuzzer::length = 0;

bool Fuzzer::run(std::string application)
{
    Log::info(VerbosityLevel::Standard, "Fuzzing EIP and finding JMP ESP...\n");
    for (Fuzzer::length = 0; Fuzzer::length <= 0x20000; Fuzzer::length += 0x10)
    {
        Log::info(VerbosityLevel::Debug, "Fuzzing with length 0x%05x\n", Fuzzer::length);
        Debugger::exec(application, { Pattern::create(Fuzzer::length) }, Fuzzer::handle);
        if (Application::jmpEsp == nullptr)
        {
            Log::error(VerbosityLevel::Standard, "Fuzzer failed to find JMP ESP\n");
            return false;
        }

        if (Application::eipOffset != std::string::npos)
        {
            Log::success(VerbosityLevel::Standard, "EIP found at offset 0x%x with a padding length of 0x%x\n", Application::eipOffset, Fuzzer::length);
            break;
        }
    }

    if (Application::eipOffset == std::string::npos)
        Log::error(VerbosityLevel::Standard, "Fuzzer failed to find EIP\n");
    return Application::eipOffset != std::string::npos;
}

void Fuzzer::handle(Debugger* debugger)
{
    while(true)
    {
        // Wait for process state to change
        ProcessState status = debugger->getProcess()->wait();

        if (status.isPaused())
        {
            // If we have hit a trap, continue
            if (status.getPauseSignal() == SIGTRAP)
            {
                if (Application::jmpEsp == nullptr)
                    Application::jmpEsp = Fuzzer::findJumpEsp(debugger);

                debugger->pContinue();

                if (Application::jmpEsp == nullptr)
                    return;
                continue;
            }

            // If we have hit a segmentation fault, then note the fuzz length
            // and stop fuzzing.
            if (status.getPauseSignal() == SIGSEGV)
            {
                user_regs_struct registers;
                debugger->getRegisters(&registers);
                Application::eipOffset = Pattern::offset(registers.eip, Fuzzer::length);
                debugger->pKill();
                return;
            }
        }
        else if (status.hasExited())
            return;
    }
}

void* Fuzzer::findJumpEsp(Debugger* debugger)
{
    Process* process = debugger->getProcess();
    void* address = process->search({ 0xFF, 0xE4 });
    if (address != nullptr)
        Log::success(VerbosityLevel::Standard, "Found JMP ESP at 0x%lx\n", address);
    return address;
}

int Fuzzer::getLength()
{
    return Fuzzer::length;
}
