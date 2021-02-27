#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include "Debugger/Process/Process.hpp"
#include "Debugger/Process/Mapping/Mapping.hpp"
#include "Utility/Filesystem/File.hpp"

struct MappingIterator;

class Application
{
public:
    static bool isSystemAslrEnabled();
public:
    static pid_t processId;
    static std::string name;
    static std::string exploitName;
    static size_t eipOffset;
    static void* jmpEsp;
    static std::vector<uint8_t> badCharacters;
    static std::string shell;
    static MappingIterator* mappings;
};