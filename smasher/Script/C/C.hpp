#pragma once

#include <string>
#include <iostream>
#include "Script/Script.hpp"

class C: public Script
{
public:
    std::string getUsage(std::string filename);
    std::string getExtension();
    std::string build();
};
