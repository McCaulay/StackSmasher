#include <string>
#include "Stages/Fuzzer/Pattern/Pattern.hpp"

std::string Pattern::create(uint32_t length)
{
    std::string pattern = "";
    char parts[] = { 'A', 'a', '0' };
    while (pattern.length() != length)
    {
        pattern += parts[pattern.length() % 3];
        if (pattern.length() % 3 == 0)
        {
            parts[2] = (char)((int32_t)parts[2] + 1);
            if (parts[2] > '9')
            {
                parts[2] = '0';
                parts[1] = (char)((int32_t)parts[1] + 1);
                if (parts[1] > 'z')
                {
                    parts[1] = 'a';
                    parts[0] = (char)((int32_t)parts[0] + 1);
                    if (parts[0] > 'Z')
                        parts[0] = 'A';
                }
            }
        }
    }
    return pattern;
}

size_t Pattern::offset(std::string value, uint32_t length)
{
    return Pattern::create(length).find(value);
}

size_t Pattern::offset(uint32_t value, uint32_t length)
{
    return Pattern::offset(std::string((char*)&value, 4), length);
}
