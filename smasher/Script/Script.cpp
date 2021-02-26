#include "Script/Script.hpp"

Script::Script()
{
    this->paddingCharacter = 'A';
    this->paddingCount = 0;
    this->eip = 0;
    this->nopSledCount = 0;
    this->payload = "";
}

void Script::save(std::string filename)
{
    filename = filename == "" ? Application::name + "-exploit" : filename;
    File::writeString(filename + "." + this->getExtension(), this->build());
    Log::success(VerbosityLevel::Standard, "Saved exploit to " + filename + "." + this->getExtension() + "\n");

    std::string usage = "Usage: ";
    if (Log::colour)
        usage += std::string(LOG_COLOUR_ORANGE);
    usage += this->getUsage(filename);
    if (Log::colour)
        usage += std::string(LOG_COLOUR_NONE);
    Log::info(VerbosityLevel::Standard,  usage + "\n");
}

std::string Script::getFormattedPaddingCharacter()
{
    // If the character is not a standard, visible ascii character, print as a hex string.
    std::string paddingCharacterFormatted = std::string(1, this->paddingCharacter);
    if ((uint8_t)this->paddingCharacter < 0x20 || (uint8_t)this->paddingCharacter > 0x7E)
    {
        char buff[5];
        sprintf(buff, "\\x%02x", (uint8_t)this->paddingCharacter);
        paddingCharacterFormatted = std::string(buff);
    }
    return paddingCharacterFormatted;
}

std::string Script::getEipAddress()
{
    // Format the JMP ESP address
    char eipAddress[11];
    sprintf(eipAddress, "0x%04x", this->eip);
    return std::string(eipAddress);
}

std::string Script::getJmpEsp()
{
    // Format JMP ESP as a hex string
    std::string jmpEsp = "";
    for (int32_t i = 0; i < 4; i++)
    {
        char buff[5];
        sprintf(buff, "\\x%02x", (uint8_t)((this->eip >> (8* i)) & 0xff));
        jmpEsp += std::string(buff);
    }
    return jmpEsp;
}

std::string Script::getFormattedPayload()
{
    // Format payload as a hex string
    std::string payloadFormatted = "";
    for (int32_t i = 0; i < this->payload.length(); i++)
    {
        char buff[5];
        sprintf(buff, "\\x%02x", (uint8_t)this->payload[i]);
        payloadFormatted += std::string(buff);
    }
    return payloadFormatted;
}