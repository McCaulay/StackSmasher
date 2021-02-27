#include "Utility/String/String.hpp"

std::string String::getLineWithText(std::string input, std::string text)
{
    size_t start = input.find(text);
    if (start == std::string::npos)
        return "";
    
    size_t end = input.find("\n", start);
    return input.substr(start, end - start);
}
