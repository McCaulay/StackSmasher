#pragma once

#include <string>
#include <sys/wait.h>
#include "Stages/Fuzzer/Pattern/Pattern.hpp"
#include "Debugger/Debugger.hpp"
#include "Log/Log.hpp"

class Fuzzer
{
public:
	static bool run(std::string application);
    static void handle(Debugger* debugger);
    static void* findJumpEsp(Debugger* debugger);
    static size_t getEipOffset();
    static void* getJmpEsp();
    static int getLength();
private:
    static size_t eipOffset;
    static void* jmpEsp;
    static int length;
};