#pragma once

#include <string>

class File
{
public:
    static uint8_t* readAllBytes(std::string filepath, size_t* length);
    static uint8_t* readBytes(std::string filepath, size_t length);
    static void writeString(std::string filepath, std::string content);
    static std::string getPathFromFilepath(std::string filepath);
    static std::string getFilenameFromFilepath(std::string filepath);
    static std::string getFileFromFilename(std::string filename);
    static std::string getExtensionFromFilename(std::string filename);
    static std::string getFileFromFilepath(std::string filepath);
    static std::string getExtensionFromFilepathame(std::string filepath);
};