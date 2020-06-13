#ifndef VARIOUS_HPP
#define VARIOUS_HPP

#include <string.h>
#include <stdint.h>

class Compression
{
public:

    // Sort of RLE algo
    // Uncompress the image data compressed in 02
    static void image02Uncompression(unsigned char *compressedData, unsigned char *uncompressedData, uint16_t width, uint16_t height);
    // Compress the image data in 02 compression
    static size_t image02Compression(unsigned char *compressedData, unsigned char *uncompressedData, uint16_t width, uint16_t height);
};

#endif // VARIOUS_HPP
