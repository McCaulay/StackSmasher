#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include "Utility/Filesystem/File.hpp"

class Application
{
public:
    static bool isAslrEnabled();
public:
    static size_t eipOffset;
    static void* jmpEsp;
    static std::vector<uint8_t> badCharacters;
};