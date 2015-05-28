#include "various.hpp"
#include <iostream>
#include <vector>

// Decompress the image data compressed in 02
void Compression::image02Decompression(unsigned char *compressedData, unsigned char *decompressedData, uint16_t width, uint16_t height)
{
    uint16_t bytesProcessed(0);
    uint16_t uncompDataPosition(0);
    uint8_t counter(0), color(0);
    for (int i(0); i<height; i++)
    {
        uint16_t lineByteRemaining = width;
        while (lineByteRemaining > 0)
        {
            counter = compressedData[bytesProcessed];
            bytesProcessed ++;
            if (counter >= 0x80)
            {
                counter = counter & 0x7F;
                counter ++;
                color = compressedData[bytesProcessed];
                bytesProcessed ++;
                for (int j(0); j<counter; j++)
                {
                    decompressedData[uncompDataPosition] = color;
                    uncompDataPosition ++;
                }
            }
            else
            {
                counter ++;
                for (int j(0); j<counter; j++)
                {
                    decompressedData[uncompDataPosition] = compressedData[bytesProcessed];
                    bytesProcessed ++;
                    uncompDataPosition ++;
                }
            }
            lineByteRemaining -= counter;
        }
    }
}

// Compress the image data in 02 compression
size_t Compression::image02Compression(unsigned char *compressedData, unsigned char *decompressedData, uint16_t width, uint16_t height)
{
    uint16_t compDataPosition(0), unCompDataPosition(0);
    for (int i(0); i<height; i++)
    {
        uint16_t lineByteProcessed = 0;
        while (lineByteProcessed < width)
        {
            uint8_t color(0), counter(0);
            color = decompressedData[unCompDataPosition];
            if (decompressedData[unCompDataPosition+1] != color)
            {
                while (counter < 0x7F &&
                       lineByteProcessed + counter < width &&
                       decompressedData[unCompDataPosition + counter] != decompressedData[unCompDataPosition + counter+1])
                {
                    counter ++;
                }
                // In case stop at width-1 because last color same as first next line
                if (lineByteProcessed+counter == width-1)
                {
                    counter ++;
                }
                compressedData[compDataPosition] = counter - 1;
                compDataPosition ++;
                for (int j(0); j<counter; j++)
                {
                    compressedData[compDataPosition] = decompressedData[unCompDataPosition];
                    compDataPosition ++;
                    unCompDataPosition ++;
                }
                lineByteProcessed += counter;
            }
            else
            {
                while (counter < 0x7E &&
                       lineByteProcessed + counter +1 < width &&
                       decompressedData[unCompDataPosition + counter] == decompressedData[unCompDataPosition + counter+1])
                {
                    counter ++;
                }
                compressedData[compDataPosition] = counter | 0x80;
                compDataPosition ++;
                compressedData[compDataPosition] = color;
                compDataPosition ++;
                unCompDataPosition += counter+1;
                lineByteProcessed += counter+1;
            }
        }
    }
    return compDataPosition;
}

// Decompress the image data compressed in 04
void Compression::image04Decompression(unsigned char *compressedData, unsigned char *decompressedData, size_t compressedLengh)
{
    unsigned char *uncompBuffer = new unsigned char[4096];
    for (int i(0); i<0xFEE; i++)
    {
        uncompBuffer[i] = 0x20;
    }
    uint16_t bytesProcessed = 0;
    uint8_t byte1 = 0x00;
    uint8_t byte2 = 0x00;
    uint16_t uncompBufferPosition = 0x0FEE;
    uint16_t bitMask = 0x0000;
    uint16_t uncompDataPosition = 0x0000;
    uint16_t bufferPositionToCopy = 0x0000;
    uint8_t numberOfTurn = 0x00;
    while (bytesProcessed < compressedLengh)
    {
        bitMask = bitMask >> 1;
        if ((bitMask & 0xFF00) == 0x0000)
        {
            byte1 = compressedData[bytesProcessed];
            bytesProcessed += 1;
            bitMask = byte1 | 0xFF00;
        }
        if ((bitMask & 0x01) == 0x01)
        {
            byte1 = compressedData[bytesProcessed];
            bytesProcessed += 1;
            uncompBuffer[uncompBufferPosition] = byte1;
            uncompBufferPosition += 1;
            uncompBufferPosition = uncompBufferPosition & 0x0FFF;
            decompressedData[uncompDataPosition] = byte1;
            uncompDataPosition += 1;
        }
        else
        {
            byte1 = compressedData[bytesProcessed];
            bytesProcessed += 1;
            byte2 = compressedData[bytesProcessed];
            bytesProcessed += 1;
            numberOfTurn = (byte2 & 0x0F) + 2;
            bufferPositionToCopy = ((byte2 & 0xF0) << 4) | byte1;
            for (uint8_t i(0); i <= numberOfTurn; i++)
            {
                uncompBuffer[uncompBufferPosition] = uncompBuffer[(bufferPositionToCopy + i) & 0x0FFF];
                uncompBufferPosition += 1;
                uncompBufferPosition = uncompBufferPosition & 0x0FFF;
                decompressedData[uncompDataPosition] = uncompBuffer[(bufferPositionToCopy + i) & 0x0FFF];
                uncompDataPosition += 1;
            }
        }
    }
    delete[] uncompBuffer;
}

// Decompress the image data compressed in 08
void Compression::image08Decompression(unsigned char *compressedData, unsigned char *decompressedData, size_t compressedSize, size_t decompressedSize)
{
    unsigned char data1[256] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                                0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
                                0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
                                0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
                                0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
                                0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
                                0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,
                                0x0C, 0x0C, 0x0C, 0x0C, 0x0D, 0x0D, 0x0D, 0x0D, 0x0E, 0x0E, 0x0E, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F,
                                0x10, 0x10, 0x10, 0x10, 0x11, 0x11, 0x11, 0x11, 0x12, 0x12, 0x12, 0x12, 0x13, 0x13, 0x13, 0x13,
                                0x14, 0x14, 0x14, 0x14, 0x15, 0x15, 0x15, 0x15, 0x16, 0x16, 0x16, 0x16, 0x17, 0x17, 0x17, 0x17,
                                0x18, 0x18, 0x19, 0x19, 0x1A, 0x1A, 0x1B, 0x1B, 0x1C, 0x1C, 0x1D, 0x1D, 0x1E, 0x1E, 0x1F, 0x1F,
                                0x20, 0x20, 0x21, 0x21, 0x22, 0x22, 0x23, 0x23, 0x24, 0x24, 0x25, 0x25, 0x26, 0x26, 0x27, 0x27,
                                0x28, 0x28, 0x29, 0x29, 0x2A, 0x2A, 0x2B, 0x2B, 0x2C, 0x2C, 0x2D, 0x2D, 0x2E, 0x2E, 0x2F, 0x2F,
                                0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F};

    unsigned char data2[256] = {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
                                0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
                                0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
                                0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
                                0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
                                0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
                                0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
                                0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
                                0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
                                0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
                                0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
                                0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
                                0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
                                0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
                                0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
                                0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08};

    unsigned char *colorBuffer = new unsigned char[0x103B];
    std::vector<uint16_t> buffer1(0x03AD);
    std::vector<uint16_t> buffer2(0x0273);
    std::vector<uint16_t> buffer3(0x0274);
    uint16_t compPosition = 0x0000;
    uint16_t uncompPosition = 0x0000;
    uint16_t colorVariable = 0x0000;
    for (uint16_t i(0); i<0x013A; i++)
    {
        buffer3[i] = 0x0001;
        buffer2[i] = 0x0273 + i;
        buffer1[i + 0x0273] = i;
    }
    uint16_t j = 0x0000;
    for (uint16_t i(0x013A); i <= 0x0272; i++)
    {
        buffer3[i] = buffer3[j] + buffer3[j+1];
        buffer2[i] = j;
        buffer1[j+1] = i;
        buffer1[j] = i;
        j+=2;
    }
    buffer3[0x0273] = 0xFFFF;
    buffer1[0x0272] = 0x0000;
    for (uint16_t i(0); i<0x0FC4; i++)
    {
        colorBuffer[i] = 0x20;
    }
    uint16_t variable = 0x0FC4;
    uint8_t byte = 0x00;
    uint16_t word = 0x0000;
    while (uncompPosition < decompressedSize)
    {
        uint16_t variable2 = buffer2[0x0272];
        while (variable2 < 0x0273)
        {
            uint16_t local1 = 0x0000;
            while (byte <= 0x08)
            {
                uint16_t local3 = 0x0000;
                if (compPosition < compressedSize)
                {
                    local3 = compressedData[compPosition] & 0x00FF;
                    compPosition ++;
                }
                else
                {
                    local3 = 0x0000;
                }
                word = (local3 << (0x08 - byte)) | word;
                byte += 8;
            }
            uint16_t local2 = word;
            word = word << 1;
            byte --;
            if ((local2 & 0x8000) != 0)
            {
                local1 = 0x0001;
            }
            else
            {
                local1 = 0x0000;
            }
            variable2 = buffer2[variable2 + local1];
        }
        variable2 -= 0x0273;
/*
        if (buffer3[0x0272] == 0x8000)
        {
            // sub_31_60D start
            int16_t local1 = 0x0000;
            uint16_t local2 = 0x0000;
            uint16_t local3 = 0x0000;
            int16_t local4 = 0x0000;
            int16_t counter = 0x0000;
            while (counter < 0x0273)
            {
                if (buffer2[counter] < 0x0273)
                {
                    buffer3[local1] = (buffer3[counter] + 1) >> 1;
                    buffer2[local1] = buffer2[counter];
                    local1 ++;
                }
                counter ++;
            }
            counter = 0x0000;
            local1 = 0x013A;
            while (local1 < 0x0273)
            {
                local4 = counter + 1;
                buffer3[local1] = buffer3[counter] + buffer3[local4];
                local2 = buffer3[local1];

                local4 = local1 - 1;
                while (buffer3[local4] > local2)
                {
                    local4 --;
                }
                local4 ++;
                local3 = (local1 - local4);
                // sub_00_7718 start
                // sub_00_7736 start
                if (local3 < 1)
                {
                    for (uint16_t i(0); i < local3; i++)
                    {
                        buffer3[local4 + 1 + local3 - 1 - i] = buffer3[local4 + local3 - 1 - i];
                    }
                }
                else
                {
                    for (uint16_t i(0); i < (local3 >> 2); i++)
                    {
                        buffer3[local4 + 1 + i*2] = buffer3[local4 + i*2];
                        buffer3[local4 + 1 + i*2+1] = buffer3[local4 + i*2+1];
                    }
                    for (uint16_t i(0); i < (local3 & 0x0003); i++)
                    {
                        buffer3[local4 + 1 + i+(local3>>2)] = buffer3[local4 + i+(local3>>2)];
                    }
                }
                // sub_00_7736 end
                // sub_00_7718 end
                buffer3[local4] = local2;
                // sub_00_7718 start
                // sub_00_7736 start
                if (local3 < 2)
                {
                    for (uint16_t i(0); i < local3; i++)
                    {
                        buffer2[local4 + 1 + local3 - 1 - i] = buffer2[local4 + local3 - 1 - i];
                    }
                }
                else
                {
                    for (uint16_t i(0); i < (local3 >> 2); i++)
                    {
                        buffer2[local4 + 1 + i*2] = buffer2[local4 + i*2];
                    }
                    for (uint16_t i(0); i < (local3 & 0x0003); i++)
                    {
                        buffer2[local4 + 1 + i] = buffer2[local4 + i];
                    }
                }
                // sub_00_7736 end
                // sub_00_7718 end
                buffer2[local4] = counter;
                counter += 2;
                local1 ++;
            }
            counter = 0x0000;
            while (counter < 0x0273)
            {
                local4 = buffer2[counter];
                if (local4 < 0x0273)
                {
                    buffer1[local4 + 1] = counter;
                }
                buffer1[local4] = counter;
                counter ++;
            }
            // sub_31_60D end
       }
*/
        uint16_t variable3 = buffer1[variable2 + 0x0273];
        do
        {
            buffer3[variable3] += 1;
            uint16_t local1 = buffer3[variable3];
            uint16_t local2 = variable3 + 1;
            if (buffer3[local2] < local1)
            {
                while (buffer3[local2] < local1)
                {
                    local2 ++;
                }
                local2 --;
                buffer3[variable3] = buffer3[local2];
                buffer3[local2] = local1;
                uint16_t local3 = buffer2[variable3];
                buffer1[local3] = local2;
                if (local3 < 0x0273)
                {
                    buffer1[local3 + 1] = local2;
                }
                uint16_t local4 = buffer2[local2];
                buffer2[local2] = local3;
                buffer1[local4] = variable3;
                if (local4 < 0x0273)
                {
                    buffer1[local4 + 1] = variable3;
                }
                buffer2[variable3] = local4;
                variable3 = local2;
            }
            variable3 = buffer1[variable3];
        }while (variable3 != 0x0000);
        colorVariable = variable2;
        if (colorVariable < 0x0100)
        {
            uint8_t colorByte = colorVariable & 0x00FF;
            decompressedData[uncompPosition] = colorByte;
            colorBuffer[variable] = colorByte;
            variable = (variable + 1) & 0x0FFF;
            uncompPosition ++;
        }
        else
        {
            while (byte <= 0x08)
            {
                uint16_t local = 0x0000;
                if (compPosition < compressedSize)
                {
                    local = compressedData[compPosition] & 0x00FF;
                    compPosition ++;
                }
                else
                {
                    local = 0x0000;
                }
                word = word | (local << (8 - byte));
                byte += 8;
            }
            uint16_t local2 = word;
            word = word << 8;
            byte += 0xF8;
            uint16_t local3 = local2 >> 8;
            uint16_t local4 = (data1[local3] & 0x00FF) << 6;
            uint16_t local5 = (data2[local3] & 0x00FF) - 2;
            while (local5 != 0)
            {
                local5 --;
                uint16_t local6 = 0x0000;
                while (byte <= 0x08)
                {
                    uint16_t local8 = 0x0000;
                    if (compPosition < compressedSize)
                    {
                        local8 = compressedData[compPosition] & 0x00FF;
                        compPosition ++;
                    }
                    else
                    {
                        local8 = 0x0000;
                    }
                    word = (local8 << (0x08 - byte)) | word;
                    byte += 8;
                }
                uint16_t local7 = word;
                word = word << 1;
                byte --;
                if ((local7 & 0x8000) != 0)
                {
                    local6 = 0x0001;
                }
                else
                {
                    local6 = 0x0000;
                }
                local3 = (local3 << 1) + local6;
            }
            uint16_t colBufferPos = (variable - ((local3 & 0x003F) | local4) - 1) & 0x0FFF;
            uint16_t turnNumber = colorVariable + 0xFF03;
            for (uint16_t i(0); i<turnNumber; i++)
            {
                colorVariable = colorBuffer[(colBufferPos + i) & 0x0FFF] & 0x00FF;
                uint8_t colorByte = colorVariable & 0x00FF;
                decompressedData[uncompPosition] = colorByte;
                colorBuffer[variable] = colorByte;
                variable = (variable + 1) & 0x0FFF;
                uncompPosition ++;
            }
        }
    }
    delete[] colorBuffer;
}

// Decompress the data compressed in RLE
void Compression::rleDecompression(unsigned char *compressedData, unsigned char *decompressedData, size_t length)
{
    unsigned int i(0), o(0);
    unsigned char countByte;
    unsigned char valueByte;
    while (i < length)
    {
        countByte = compressedData[i];
        if (countByte > 0x7F)
        {
            unsigned int count = countByte - 0x7F;
            for (unsigned int j(0); j < count; j++)
            {
                valueByte = compressedData[i+1];
                decompressedData[o] = valueByte;
                i++;
                o++;
            }
        }
        else
        {
            valueByte = compressedData[i+1];
            unsigned int count = countByte + 1;
            for (unsigned int j(0); j < count; j++)
            {
                decompressedData[o] = valueByte;
                o++;
            }
            i+=2;
        }
    }
}

// Decrypt encrypt Arena inf file
void Compression::encryptDecryptINF(char *sourceData, size_t length)
{
    unsigned char cryptKey[8] = {0xEA, 0x7B, 0x4E, 0xBD, 0x19, 0xC9, 0x38, 0x99};
    uint8_t counter(0);
    uint8_t keyIndex(0);
    for (size_t i(0); i<length; i++)
    {
        sourceData[i] = sourceData[i] ^ (counter + cryptKey[keyIndex]);
        counter ++;
        keyIndex = (keyIndex + 1) & 0x07;
    }
}
