#ifndef VARIOUS_HPP
#define VARIOUS_HPP

#include <string.h>
#include <stdint.h>

class Compression
{
public:

    // Sort of RLE algo
    // Decompress the image data compressed in 02
    static void image02Decompression(unsigned char *compressedData, unsigned char *decompressedData, uint16_t width, uint16_t height);
    // Compress the image data in 02 compression
    static size_t image02Compression(unsigned char *compressedData, unsigned char *decompressedData, uint16_t width, uint16_t height);

    // Compress the image data in 04
    static size_t image04Compression(unsigned char *compressedData, unsigned char *decompressedData, size_t decompressedLengh);

    // Decompress the image data compressed in 08
    static void image08Decompression(unsigned char *compressedData, unsigned char *decompressedData, size_t compressedSize, size_t decompressedSize);
};

#endif // VARIOUS_HPP
