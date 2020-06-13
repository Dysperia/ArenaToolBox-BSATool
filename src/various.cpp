#include "various.hpp"
#include <iostream>
#include <vector>

// Uncompress the image data compressed in 02
void Compression::image02Uncompression(unsigned char *compressedData, unsigned char *uncompressedData, uint16_t width, uint16_t height)
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
                    uncompressedData[uncompDataPosition] = color;
                    uncompDataPosition ++;
                }
            }
            else
            {
                counter ++;
                for (int j(0); j<counter; j++)
                {
                    uncompressedData[uncompDataPosition] = compressedData[bytesProcessed];
                    bytesProcessed ++;
                    uncompDataPosition ++;
                }
            }
            lineByteRemaining -= counter;
        }
    }
}
// Compress the image data in 02 compression
size_t Compression::image02Compression(unsigned char *compressedData, unsigned char *uncompressedData, uint16_t width, uint16_t height)
{
    uint16_t compDataPosition(0), unCompDataPosition(0);
    for (int i(0); i<height; i++)
    {
        uint16_t lineByteProcessed = 0;
        while (lineByteProcessed < width)
        {
            uint8_t color(0), counter(0);
            color = uncompressedData[unCompDataPosition];
            if (uncompressedData[unCompDataPosition+1] != color)
            {
                while (counter < 0x7F &&
                       lineByteProcessed + counter < width &&
                       uncompressedData[unCompDataPosition + counter] != uncompressedData[unCompDataPosition + counter+1])
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
                    compressedData[compDataPosition] = uncompressedData[unCompDataPosition];
                    compDataPosition ++;
                    unCompDataPosition ++;
                }
                lineByteProcessed += counter;
            }
            else
            {
                while (counter < 0x7E &&
                       lineByteProcessed + counter +1 < width &&
                       uncompressedData[unCompDataPosition + counter] == uncompressedData[unCompDataPosition + counter+1])
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
