#pragma once

#include <string>
#include "Stages/Application.hpp"
#include "Stages/Payload.hpp"
#include "Debugger/Debugger.hpp"
#include "Log/Log.hpp"

class BadCharacters
{
public:
	static bool run(std::string application);
    static void handle(Debugger* debugger);
private:
    static std::string getPayload();
private:
    static uint32_t current;
    static bool hitInitialBreakpoint;
    static bool completed;
};