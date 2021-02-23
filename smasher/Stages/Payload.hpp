#pragma once

#include <string>
#include <algorithm>
#include "Stages/Application.hpp"
#include "Log/Log.hpp"

class Payload
{
public:
    static Payload* builder();
    Payload();
    std::string get();
    Payload* padding(char character = 'A');
    Payload* eip();
    Payload* nopSled(uint32_t length = 0x20);
    Payload* append(std::string input);
    Payload* badCharacters();
private:
    std::string buffer;
};