#include "Stages/Fuzzer/Fuzzer.hpp"

size_t Fuzzer::eipOffset = std::string::npos;
void* Fuzzer::jmpEsp = nullptr;
int Fuzzer::length = 0;

bool Fuzzer::run(std::string application)
{
    Log::info("Fuzzing EIP and finding JMP ESP...\n");
    for (Fuzzer::length = 0; Fuzzer::length <= 0x20000; Fuzzer::length += 0x10)
    {
        Debugger::exec(application, { Pattern::create(Fuzzer::length) }, Fuzzer::handle);
        if (Fuzzer::jmpEsp == nullptr)
            return false;

        if (Fuzzer::eipOffset != std::string::npos)
        {
            Log::success("EIP found at offset 0x%x with length of 0x%x\n", Fuzzer::eipOffset, Fuzzer::length);
            break;
        }
    }

    if (Fuzzer::eipOffset == std::string::npos)
        Log::error("Fuzzer failed to find EIP\n");
    return Fuzzer::eipOffset != std::string::npos;
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
                if (Fuzzer::jmpEsp == nullptr)
                    Fuzzer::jmpEsp = Fuzzer::findJumpEsp(debugger);

                debugger->pContinue();

                if (Fuzzer::jmpEsp == nullptr)
                    return;
                continue;
            }

            // If we have hit a segmentation fault, then note the fuzz length
            // and stop fuzzing.
            if (status.getPauseSignal() == SIGSEGV)
            {
                user_regs_struct registers;
                debugger->getRegisters(&registers);
                Fuzzer::eipOffset = Pattern::offset(registers.eip, Fuzzer::length);
                return;
            }
        }
        else if (status.hasExited())
            return;
    }
}

void* Fuzzer::findJumpEsp(Debugger* debugger)
{
    Mapping::MappingIterator* mappings = debugger->getProcess()->getMappings();
    if (mappings == nullptr)
        return 0;

    // Iterate over mappings
    Mapping* mapping = nullptr;
    while ((mapping = Mapping::next(mappings)) != nullptr)
    {
        if (mapping->is_x)
        {
            void* address = mapping->search({ 0xFF, 0xE4 });
            if (address != nullptr)
                Log::success("Found JMP ESP at 0x%lx\n", address);
            else
                Log::error("Fuzzer failed to find JMP ESP\n");
            
            Mapping::freeAll(mappings);
            return address;
        }
    }
    Mapping::freeAll(mappings);
    return nullptr;
}

size_t Fuzzer::getEipOffset()
{
    return Fuzzer::eipOffset;
}

void* Fuzzer::getJmpEsp()
{
    return Fuzzer::jmpEsp;
}

int Fuzzer::getLength()
{
    return Fuzzer::length;
}
