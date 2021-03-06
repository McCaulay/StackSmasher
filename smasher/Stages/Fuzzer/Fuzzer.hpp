#pragma once

#include <string>
#include <sys/wait.h>
#include "Stages/Application.hpp"
#include "Stages/Fuzzer/Pattern/Pattern.hpp"
#include "Debugger/Debugger.hpp"
#include "Log/Log.hpp"

class Fuzzer
{
public:
	static bool run(std::string application);
    static void handle(Debugger* debugger);
    static void* findJumpEsp(Debugger* debugger);
    static int getLength();
private:
    static int length;
};