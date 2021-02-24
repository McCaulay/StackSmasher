#include "Log/Log.hpp"

VerbosityLevel Log::verbose = VerbosityLevel::Standard;
bool Log::colour = true;

std::string Log::getPrefix(std::string icon, const std::string colour)
{
    std::string prefix = "";
    if (Log::colour)
        prefix += colour;

    prefix += "[" + icon + "] ";

    if (Log::colour)
        prefix += LOG_COLOUR_NONE;
    return prefix;
}

void Log::print(VerbosityLevel level, std::string format, ...)
{
    if (Log::verbose >= level)
    {
        if (Log::colour)
            format = LOG_COLOUR_NONE + format;
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
        format = Log::getPrefix("*", LOG_COLOUR_BLUE) + format;
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
        format = Log::getPrefix("!", LOG_COLOUR_ORANGE) + format;
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
        format = Log::getPrefix("+", LOG_COLOUR_GREEN) + format;
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
        format = Log::getPrefix("-", LOG_COLOUR_RED) + format;
        va_list argList;
        va_start(argList, format);
        vprintf(format.c_str(), argList);
        va_end(argList);
    }
}