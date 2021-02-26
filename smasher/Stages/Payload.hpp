#pragma once

#include <string>
#include <algorithm>
#include "Stages/Application.hpp"
#include "Log/Log.hpp"
#include "Script/Script.hpp"

class Payload
{
public:
    static Payload* builder(Script* script = nullptr);
    Payload(Script* script = nullptr);
    std::string get();
    Payload* padding(char character = 'A');
    Payload* eip();
    Payload* nopSled(uint32_t length = 0x50);
    Payload* payload(std::string input);
    Payload* badCharacters();
private:
    Script* script;
    std::string buffer;
};