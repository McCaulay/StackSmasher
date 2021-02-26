#pragma once

#include <string>
#include "Stages/Application.hpp"
#include "Utility/Filesystem/File.hpp"
#include "Log/Log.hpp"

class Script
{
public:
    Script();
    void save(std::string filename);
    virtual std::string getUsage(std::string filename) = 0;
    virtual std::string getExtension() = 0;
    virtual std::string build() = 0;
protected:
    std::string getFormattedPaddingCharacter();
    std::string getEipAddress();
    std::string getJmpEsp();
    std::string getFormattedPayload();
public:
    char paddingCharacter;
    size_t paddingCount;
    uint32_t eip;
    size_t nopSledCount;
    std::string payload;
};