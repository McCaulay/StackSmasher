#pragma once

#include <string>
#include "Stages/Application.hpp"
#include "Stages/Fuzzer/Pattern/Pattern.hpp"
#include "Debugger/Debugger.hpp"
#include "Log/Log.hpp"

class Protection
{
public:
	static bool run(std::string application);
    static void handle(Debugger* debugger);
};