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
    uint8_t* data = (uint8_t*)malloc(len);

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
    uint8_t* data = (uint8_t*)malloc(length);

    // Read file into memory
    rewind(file);
    fread(data, length, 1, file);

    // Close file
    fclose(file);

    return data;
}
