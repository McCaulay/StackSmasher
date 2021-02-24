#pragma once

#include "argparse.hpp"
#include <cstring>

class Execute
{
public:
    static uint8_t* getPayload(size_t* length, argparse::ArgumentParser program);
};