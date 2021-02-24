#include "Log/Log.hpp"

VerbosityLevel Log::verbose = VerbosityLevel::Standard;

void Log::print(VerbosityLevel level, std::string format, ...)
{
    if (Log::verbose >= level)
    {
        format = std::string(LOG_COLOUR_NONE) + format;
        va_list argList;
        va_start(argList, format);
        vprintf(format.c_str(), argList);
        va_end(argList);
    }
}

void Log::info(VerbosityLevel level, std::string format, ...)
{
    if (Log::verbose >= level)
    {
        format = std::string(LOG_COLOUR_BLUE) + "[*] " + std::string(LOG_COLOUR_NONE) + format;
        va_list argList;
        va_start(argList, format);
        vprintf(format.c_str(), argList);
        va_end(argList);
    }
}

void Log::warning(VerbosityLevel level, std::string format, ...)
{
    if (Log::verbose >= level)
    {
        format = std::string(LOG_COLOUR_ORANGE) + "[!] " + std::string(LOG_COLOUR_NONE) + format;
        va_list argList;
        va_start(argList, format);
        vprintf(format.c_str(), argList);
        va_end(argList);
    }
}

void Log::success(VerbosityLevel level, std::string format, ...)
{
    if (Log::verbose >= level)
    {
        format = std::string(LOG_COLOUR_GREEN) + "[+] " + std::string(LOG_COLOUR_NONE) + format;
        va_list argList;
        va_start(argList, format);
        vprintf(format.c_str(), argList);
        va_end(argList);
    }
}

void Log::error(VerbosityLevel level, std::string format, ...)
{
    if (Log::verbose >= level)
    {
        format = std::string(LOG_COLOUR_RED) + "[-] " + std::string(LOG_COLOUR_NONE) + format;
        va_list argList;
        va_start(argList, format);
        vprintf(format.c_str(), argList);
        va_end(argList);
    }
}

void Log::exploit(VerbosityLevel level, std::string format, ...)
{
    if (Log::verbose >= level)
    {
        va_list argList;
        va_start(argList, format);
        vprintf(format.c_str(), argList);
        va_end(argList);
    }
}
