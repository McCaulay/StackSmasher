#include "Debugger/Process/Process.hpp"

void Process::exec(std::string path, std::vector<std::string> arguments)
{
    std::string app = File::getPathFromFilepath(path);
    std::vector<char*> args;

	args.push_back((char*)app.c_str());

    // Append each argument
    for(std::vector<std::string>::iterator it = arguments.begin(); it != arguments.end(); ++it)
        args.push_back((char*)((*it).c_str()));

	args.push_back(NULL);
    execv(path.c_str(), &args.front());
}

std::string Process::execWithOutput(std::string path, std::vector<std::string> arguments)
{
    int pipes[2];
    pipe(pipes);
    
    pid_t child = fork();
    if (child == 0)
    {
        dup2(pipes[1], STDOUT_FILENO);
        close(pipes[0]);
        close(pipes[1]);
        Process::exec(path, arguments);
        exit(0);
    }

    // Close pipe
    close(pipes[1]);

    Process* process = new Process(child);

    char output[4096];
    int bytesRead = ::read(pipes[0], output, sizeof(output));

    // Wait for process state to exit
    process->wait();

    // Cleanup
    delete process;

    return std::string(output, bytesRead);
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

MappingIterator* Process::getMappings()
{
    if (Application::mappings != nullptr)
    {
        Application::mappings->current = Application::mappings->head;
        Application::mappings = Mapping::parse(this->id);
        return Application::mappings;
    }
    Application::processId = this->id;
    Application::mappings = Mapping::parse(this->id);
    return Application::mappings;
}

uint8_t* Process::read(void* address, size_t length)
{
    MappingIterator* mappings = this->getMappings();
    if (Application::mappings == nullptr)
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
            return data;
        }
    }
    return nullptr;
}

void Process::write(void* address, uint8_t* buffer, size_t length)
{
    MappingIterator* mappings = this->getMappings();
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
            return;
        }
    }
    return;
}

void* Process::search(std::vector<uint8_t> pattern)
{
    MappingIterator* mappings = this->getMappings();
    if (mappings == nullptr)
    {
        Log::error(VerbosityLevel::Standard, "Failed to read process mappings\n");
        return nullptr;
    }

    // Iterate over mappings
    Mapping* mapping = nullptr;
    while ((mapping = Mapping::next(mappings)) != nullptr)
    {
        if (!mapping->is_x)
            continue;

        void* address = mapping->search(pattern);
        if (address != nullptr)
            return address;
    }
    return nullptr;
}
