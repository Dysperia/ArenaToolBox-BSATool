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

    // Compress the image data in 04
    static size_t image04Compression(unsigned char *compressedData, unsigned char *uncompressedData, size_t uncompressedLengh);

    // Uncompress the image data compressed in 08
    static void image08Uncompression(unsigned char *compressedData, unsigned char *uncompressedData, size_t compressedSize, size_t uncompressedSize);
};

#endif // VARIOUS_HPP
