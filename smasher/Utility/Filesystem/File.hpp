#pragma once

#include <string>

class File
{
public:
    static uint8_t* readAllBytes(std::string filepath, size_t* length);
};