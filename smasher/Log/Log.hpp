#pragma once

#include <string>
#include <stdio.h>
#include <stdarg.h>

#define LOG_COLOUR_NONE "\033[0m"
#define LOG_COLOUR_BLACK "\033[0;30m"
#define LOG_COLOUR_RED "\033[0;31m"
#define LOG_COLOUR_GREEN "\033[0;32m"
#define LOG_COLOUR_ORANGE "\033[0;33m"
#define LOG_COLOUR_BLUE "\033[0;34m"
#define LOG_COLOUR_PURPLE "\033[0;35m"
#define LOG_COLOUR_CYAN "\033[0;36m"
#define LOG_COLOUR_GRAY "\033[1;30m"
#define LOG_COLOUR_YELLOW "\033[1;33m"
#define LOG_COLOUR_WHITE "\033[1;37m"
#define LOG_COLOUR_RED_LIGHT "\033[1;31m"
#define LOG_COLOUR_GREEN_LIGHT "\033[1;32m"
#define LOG_COLOUR_BLUE_LIGHT "\033[1;34m"
#define LOG_COLOUR_PURPLE_LIGHT "\033[1;35m"
#define LOG_COLOUR_CYAN_LIGHT "\033[1;36m"
#define LOG_COLOUR_GRAY_LIGHT "\033[0;37m"

enum VerbosityLevel
{
    None = 0,
    Standard = 1,
    Verbose = 2,
    VeryVerbose = 3,
    Debug = 4,
};

class Log
{
public:
    static void print(VerbosityLevel level, std::string format, ...);
    static void info(VerbosityLevel level, std::string format, ...);
    static void warning(VerbosityLevel level, std::string format, ...);
    static void success(VerbosityLevel level, std::string format, ...);
	static void error(VerbosityLevel level, std::string format, ...);
    static VerbosityLevel verbose;
    static bool colour;
private:
    static std::string getPrefix(std::string icon, const std::string colour);
};