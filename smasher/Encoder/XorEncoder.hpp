#pragma once

#include <algorithm>
#include <random>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <cstdint>
#include <vector>
#include "Log/Log.hpp"

// See: https://rastating.github.io/creating-a-custom-shellcode-encoder/

class XorEncoder
{
public:
	static std::string encode(uint8_t* buffer, size_t length, uint8_t* badCharacters, size_t badCharactersLength);
private:
    // See encoders/xor.asm
    static std::string getStub();

    static bool bufferContainsAny(uint8_t* buffer, size_t bufferLength, uint8_t* other, size_t otherLength);
    static uint8_t findValidXorByte(uint8_t* buffer, size_t length, uint8_t* badCharacters, size_t badCharactersLength);
    static void findAndReplaceAll(std::string &data, std::string toSearch, std::string replaceStr);
};