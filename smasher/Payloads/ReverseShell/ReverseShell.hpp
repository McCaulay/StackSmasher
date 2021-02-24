#pragma once

#include "argparse.hpp"
#include <cstring>

class ReverseShell
{
public:
    static uint8_t* getPayload(size_t* length, argparse::ArgumentParser program);
};