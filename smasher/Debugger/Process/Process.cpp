#include "Debugger/Process/Process.hpp"

void Process::exec(std::string path, std::vector<std::string> arguments)
{
    std::vector<char*> args = Process::getArguments(path, arguments);
	execv(args[0], &args.front());
}

void Process::execFork(std::string path, std::vector<std::string> arguments)
{
    std::vector<char*> args = Process::getArguments(path, arguments);

    // Redirect stdout and stderr to /dev/null
    int devnull = open("/dev/null", O_WRONLY);
    dup2(devnull, 1); // stdout
    dup2(devnull, 2); // stderr
    close(devnull);

	execv(args[0], &args.front());
    exit(0);
}

std::vector<char*> Process::getArguments(std::string path, std::vector<std::string> arguments)
{
    std::vector<char*> args;
	args.push_back((char*)path.c_str());

    // Append each argument
    for(std::vector<std::string>::iterator it = arguments.begin(); it != arguments.end(); ++it)
        args.push_back((char*)((*it).c_str()));

	args.push_back(NULL);
    return args;
}

Process::Process(pid_t pid)
{
    this->id = pid;
}

pid_t Process::getId()
{
    return this->id;
}

ProcessState Process::wait()
{
    int	status;
    waitpid(this->id, &status, 0);
    return ProcessState(status);
}

Mapping::MappingIterator* Process::getMappings()
{
    Mapping::MappingIterator* mappings = Mapping::parse(this->id);

    return mappings;
}

uint8_t* Process::read(void* address, size_t length)
{
    Mapping::MappingIterator* mappings = this->getMappings();
    if (mappings == nullptr)
    {
        Log::error(VerbosityLevel::Standard, "Failed to read process mappings\n");
        return nullptr;
    }

    // Iterate over mappings
    Mapping* mapping = nullptr;
    while ((mapping = Mapping::next(mappings)) != nullptr)
    {
        if (!mapping->is_r)
            continue;
        
        if (address >= mapping->addr_start && address <= mapping->addr_end)
        {
            uint8_t* data = mapping->read(address, length);
            Mapping::freeAll(mappings);
            return data;
        }
    }
    Mapping::freeAll(mappings);
    return nullptr;
}

void Process::write(void* address, uint8_t* buffer, size_t length)
{
    Mapping::MappingIterator* mappings = this->getMappings();
    if (mappings == nullptr)
    {
        Log::error(VerbosityLevel::Standard, "Failed to read process mappings\n");
        return;
    }

    // Iterate over mappings
    Mapping* mapping = nullptr;
    while ((mapping = Mapping::next(mappings)) != nullptr)
    {
        if (!mapping->is_w)
            continue;
        
        if (address >= mapping->addr_start && address <= mapping->addr_end)
        {
            mapping->write(address, buffer, length);
            Mapping::freeAll(mappings);
            return;
        }
    }
    Mapping::freeAll(mappings);
    return;
}

void* Process::search(std::vector<uint8_t> pattern)
{
    Mapping::MappingIterator* mappings = this->getMappings();
    if (mappings == nullptr)
    {
        Log::error(VerbosityLevel::Standard, "Failed to read process mappings\n");
        return nullptr;
    }

    Log::info(VerbosityLevel::Debug, "--------------------\n");
    Log::info(VerbosityLevel::Debug, "Process Mappings\n");

    // Iterate over mappings
    Mapping* mapping = nullptr;
    while ((mapping = Mapping::next(mappings)) != nullptr)
    {
        Log::info(VerbosityLevel::Debug, "--------------------\n");
        Log::info(VerbosityLevel::Debug, "Path: %s\n", strlen(mapping->pathname) == 0 ? "[anonym*]" : mapping->pathname);
        Log::info(VerbosityLevel::Debug, "Address Space: %p - %p\n", mapping->addr_start, mapping->addr_end);
        Log::info(VerbosityLevel::Debug, "Address Size: 0x%x\n", mapping->length);
        Log::info(VerbosityLevel::Debug, "Offset: 0x%x\n", mapping->offset);
        Log::info(VerbosityLevel::Debug, "Permissions: %s\n", mapping->perm);
        Log::info(VerbosityLevel::Debug, "Device: %s\n", mapping->dev);

        if (!mapping->is_x)
            continue;

        void* address = mapping->search(pattern);
        if (address != nullptr)
        {
            Mapping::freeAll(mappings);
            Log::info(VerbosityLevel::Debug, "--------------------\n");
            return address;
        }
    }
    Mapping::freeAll(mappings);
    Log::info(VerbosityLevel::Debug, "--------------------\n");
    return nullptr;
}
