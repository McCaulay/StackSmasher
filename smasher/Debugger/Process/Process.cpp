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
