#include "Stages/Protection/Protection.hpp"

bool Protection::run(std::string application)
{
    Debugger::exec(application, { Pattern::create(0x10) }, Protection::handle);

    // Get flags
    bool anyMappingHasAslrDisabled = false;
    bool appHasStackCanaries = false;
    bool appHasDep = false;

    // Iterate over mappings
    Application::mappings->current = Application::mappings->head;
    Mapping* mapping = nullptr;
    while ((mapping = Mapping::next(Application::mappings)) != nullptr)
    {
        if (!anyMappingHasAslrDisabled && mapping->pie == ProtectionState::Disabled)
            anyMappingHasAslrDisabled = true;

        if (!appHasStackCanaries && mapping->stackCanary == ProtectionState::Enabled && File::getFilenameFromFilepath(std::string(mapping->pathname)) == File::getFilenameFromFilepath(application))
            appHasStackCanaries = true;

        if (!appHasDep && mapping->nx == ProtectionState::Enabled && File::getFilenameFromFilepath(std::string(mapping->pathname)) == File::getFilenameFromFilepath(application))
            appHasDep = true;
    }

    // Ensure ASLR is disabled or atleast on mapping has ASLR disabled
    if (Application::isSystemAslrEnabled() && !anyMappingHasAslrDisabled)
    {
        Log::error(VerbosityLevel::Standard, "ASLR is enabled but not supported by this application.\n");
        Log::warning(VerbosityLevel::Standard, "Run `sudo bash -c \"echo 0 > /proc/sys/kernel/randomize_va_space\"` to disable ASLR system wide.\n");
        Log::warning(VerbosityLevel::Standard, "Run `sudo bash -c \"echo 2 > /proc/sys/kernel/randomize_va_space\"` to enable ASLR system wide.\n");
        return false;
    }

    // Ensure Stack Canary isn't enabled on the application
    if (appHasStackCanaries)
    {
        Log::error(VerbosityLevel::Standard, "Stack Canary is present but not supported by this application.\n");
        return false;
    }

    // Ensure DEP/NX isn't enabled on the application
    if (appHasDep)
    {
        Log::error(VerbosityLevel::Standard, "DEP/NX is enabled but ROP chains are currently not supported by this application.\n");
        return false;
    }

    return true;
}

void Protection::handle(Debugger* debugger)
{
    // Wait for process state to change
    ProcessState status = debugger->getProcess()->wait();

    // If we have hit a trap, get process mappings
    if (status.isPaused() && status.getPauseSignal() == SIGTRAP)
    {
        Log::info(VerbosityLevel::Standard, "--------------------------------------------------------------------------------------------------------------\n");
        Log::info(VerbosityLevel::Standard, "                                               Process Mappings\n");
        Log::info(VerbosityLevel::Standard, "--------------------------------------------------------------------------------------------------------------\n");

        if (Application::mappings == nullptr)
            Application::mappings = debugger->getProcess()->getMappings();

        if (Application::mappings == nullptr)
        {
            Log::error(VerbosityLevel::Standard, "Failed to read process mappings\n");
            return;
        }

        // Header
        Log::info(VerbosityLevel::Standard, "Address Space\t\tAddress Size\tPerms\tStack Canary\tNX\t\tPIE\t\tPath\n");
        Log::info(VerbosityLevel::Standard, "--------------------------------------------------------------------------------------------------------------\n");

        // Iterate over mappings
        Application::mappings->current = Application::mappings->head;
        Mapping* mapping = nullptr;
        while ((mapping = Mapping::next(Application::mappings)) != nullptr)
        {

            Log::info(VerbosityLevel::Standard, "0x%08x - 0x%08x\t0x%06x\t%s\t%s\t%s\t%s\t%s\n",
                mapping->addr_start, mapping->addr_end,
                mapping->length,
                mapping->perm,
                mapping->getProtectionStateAsString(mapping->stackCanary).c_str(),
                mapping->getProtectionStateAsString(mapping->nx).c_str(),
                mapping->getProtectionStateAsString(mapping->pie).c_str(),
                strlen(mapping->pathname) == 0 ? "[anonym*]" : mapping->pathname
            );
        }
        Log::info(VerbosityLevel::Standard, "--------------------------------------------------------------------------------------------------------------\n");

        debugger->pKill();
        return;
    }

    Log::error(VerbosityLevel::Standard, "Failed to debug application.\n");
}
