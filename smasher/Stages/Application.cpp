#include "Stages/Application.hpp"

pid_t Application::processId = 0;
std::string Application::name = "";
std::string Application::exploitName = "";
size_t Application::eipOffset = std::string::npos;
void* Application::jmpEsp = nullptr;
std::vector<uint8_t> Application::badCharacters = { 0x00 };
std::string Application::shell = "";
MappingIterator* Application::mappings = nullptr;

bool Application::isSystemAslrEnabled()
{
    uint8_t* bytes = File::readBytes("/proc/sys/kernel/randomize_va_space", 1);
    bool enabled = bytes[0] != '0';
    free(bytes);
    return enabled;
}
