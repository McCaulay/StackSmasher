#pragma once

class Pattern
{
public:
    // Generate a unique pattern for the given pattern length, so that an offset
    // can be found for a substring of the pattern.
	static std::string create(uint32_t length = 8192);

    // Returns the offset of the given value for a pattern generated at the
    // given length.
    //
    // If no matches were found, the function returns string::npos.
    static size_t offset(std::string value, uint32_t length = 8192);
    static size_t offset(uint32_t value, uint32_t length = 8192);
};