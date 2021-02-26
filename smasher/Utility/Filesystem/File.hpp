#pragma once

#include <string>

class File
{
public:
    static uint8_t* readAllBytes(std::string filepath, size_t* length);
    static uint8_t* readBytes(std::string filepath, size_t length);
    static void writeString(std::string filepath, std::string content);
};