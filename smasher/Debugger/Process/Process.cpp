#include "Debugger/Process/Process.hpp"

void Process::exec(std::string path, std::vector<std::string> arguments)
{
    std::vector<char*> args;
	args.push_back((char*)path.c_str());

    // Append each argument
    for(std::vector<std::string>::iterator it = arguments.begin(); it != arguments.end(); ++it)
        args.push_back((char*)((*it).c_str()));

	args.push_back(NULL);

    // Redirect stdout and stderr to /dev/null
    int devnull = open("/dev/null", O_WRONLY);
    dup2(devnull, 1); // stdout
    dup2(devnull, 2); // stderr
    close(devnull);

	execv(args[0], &args.front());
    exit(0);
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
    return Mapping::parse(this->id);
}

uint8_t* Process::read(void* address, size_t length)
{
    Mapping::MappingIterator* mappings = this->getMappings();
    if (mappings == nullptr)
        return nullptr;

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
        return;

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
