#include "Stages/Application.hpp"

std::string Application::name = "";
std::string Application::exploitName = "";
size_t Application::eipOffset = std::string::npos;
void* Application::jmpEsp = nullptr;
std::vector<uint8_t> Application::badCharacters = { 0x00 };
std::string Application::shell = "";

bool Application::isAslrEnabled()
{
    uint8_t* bytes = File::readBytes("/proc/sys/kernel/randomize_va_space", 1);
    bool enabled = bytes[0] != '0';
    free(bytes);
    return enabled;
}
