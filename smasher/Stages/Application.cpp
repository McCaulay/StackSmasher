#include "Stages/Application.hpp"

size_t Application::eipOffset = std::string::npos;
void* Application::jmpEsp = nullptr;
std::vector<uint8_t> Application::badCharacters = { 0x00 };
