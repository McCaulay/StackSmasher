#include "Log/Log.hpp"

LogMode Log::mode = LogMode::Verbose;

void Log::print(std::string format, ...)
{
    if (Log::mode == LogMode::Verbose)
    {
        format = std::string(LOG_COLOUR_NONE) + format;
        va_list argList;
        va_start(argList, format);
        vprintf(format.c_str(), argList);
        va_end(argList);
    }
}

void Log::info(std::string format, ...)
{
    if (Log::mode == LogMode::Verbose)
    {
        format = std::string(LOG_COLOUR_BLUE) + "[-] " + std::string(LOG_COLOUR_NONE) + format;
        va_list argList;
        va_start(argList, format);
        vprintf(format.c_str(), argList);
        va_end(argList);
    }
}

void Log::success(std::string format, ...)
{
    if (Log::mode == LogMode::Verbose)
    {
        format = std::string(LOG_COLOUR_GREEN) + "[+] " + std::string(LOG_COLOUR_NONE) + format;
        va_list argList;
        va_start(argList, format);
        vprintf(format.c_str(), argList);
        va_end(argList);
    }
}

void Log::error(std::string format, ...)
{
    if (Log::mode == LogMode::Verbose)
    {
        format = std::string(LOG_COLOUR_RED) + "[x] " + std::string(LOG_COLOUR_NONE) + format;
        va_list argList;
        va_start(argList, format);
        vprintf(format.c_str(), argList);
        va_end(argList);
    }
}

void Log::exploit(std::string format, ...)
{
    if (Log::mode == LogMode::Exploit)
    {
        va_list argList;
        va_start(argList, format);
        vprintf(format.c_str(), argList);
        va_end(argList);
    }
}
