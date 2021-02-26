#pragma once

#include <string>
#include <iostream>
#include "Stages/Application.hpp"
#include "Script/Script.hpp"

class Ruby: public Script
{
public:
    std::string getUsage(std::string filename);
    std::string getExtension();
    std::string build();
};