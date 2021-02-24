#pragma once

#include "argparse.hpp"
#include <cstring>

class BindShell
{
public:
    static uint8_t* getPayload(size_t* length, argparse::ArgumentParser program);
};