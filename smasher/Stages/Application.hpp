#pragma once

#include <vector>
#include <string>
#include <cstdint>

class Application
{
public:
    static size_t eipOffset;
    static void* jmpEsp;
    static std::vector<uint8_t> badCharacters;
};