#include "Utility/Filesystem/File.hpp"

uint8_t* File::readAllBytes(std::string filepath, size_t* length)
{
    // Open file
    FILE* file = fopen(filepath.c_str(), "rb");
    if (file == nullptr)
    {
        if (length != nullptr)
            *length = 0;
        return nullptr;
    }

    // Get size
    fseek(file, 0L, SEEK_END);
    size_t len = ftell(file);

    // Allocate memory
    uint8_t* data = new uint8_t[len];

    // Read file into memory
    rewind(file);
    fread(data, len, 1, file);

    // Close file
    fclose(file);

    // Set the length if it's not a null ptr
    if (length != nullptr)
        *length = len;

    return data;
}

uint8_t* File::readBytes(std::string filepath, size_t length)
{
    // Open file
    FILE* file = fopen(filepath.c_str(), "rb");
    if (file == nullptr)
        return nullptr;

    // Allocate memory
    uint8_t* data = new uint8_t[length];

    // Read file into memory
    rewind(file);
    fread(data, length, 1, file);

    // Close file
    fclose(file);

    return data;
}

void File::writeString(std::string filepath, std::string content)
{
    // Open file
    FILE* file = fopen(filepath.c_str(), "w");
    if (file == nullptr)
        return;

    // Write content into file
    fwrite(content.c_str(), content.length(), 1, file);

    // Close file
    fclose(file);
}

std::string File::getPathFromFilepath(std::string filepath)
{
    size_t lastSlash = filepath.find_last_of("/");
    return lastSlash != std::string::npos ? filepath.substr(0, lastSlash) : filepath;
}

std::string File::getFilenameFromFilepath(std::string filepath)
{
    size_t lastSlash = filepath.find_last_of("/");
    return lastSlash != std::string::npos ? filepath.substr(lastSlash + 1, filepath.length()) : filepath;
}

std::string File::getFileFromFilename(std::string filename)
{
    size_t lastDotIndex = filename.find_last_of("."); 
    return lastDotIndex != std::string::npos ? filename.substr(0, lastDotIndex) : filename;
}

std::string File::getExtensionFromFilename(std::string filename)
{
    size_t lastDotIndex = filename.find_last_of("."); 
    return lastDotIndex != std::string::npos ? filename.substr(lastDotIndex + 1, filename.length()) : filename;
}

std::string File::getFileFromFilepath(std::string filepath)
{
    return File::getFileFromFilename(File::getFilenameFromFilepath(filepath));
}

std::string File::getExtensionFromFilepathame(std::string filepath)
{
    return File::getExtensionFromFilename(File::getFilenameFromFilepath(filepath));
}