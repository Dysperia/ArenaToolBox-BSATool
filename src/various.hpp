#ifndef VARIOUS_HPP
#define VARIOUS_HPP

#include <string.h>

class Compression
{
public:

    // Decompress the image data compressed in 04
    static void image04Decompression(unsigned char *compressedData, unsigned char *decompressedData, size_t compressedLengh);

    // Decompress the image data compressed in 08
    static void image08Decompression(unsigned char *compressedData, unsigned char *decompressedData, size_t compressedSize, size_t decompressedSize);

    // Decompress the data compressed in RLE
    static void rleDecompression(unsigned char *compressedData, unsigned char *decompressedData, size_t length);

    // Decrypt encrypt Arena inf file
    static void encryptDecryptINF(char *sourceData, size_t length);
};

#endif // VARIOUS_HPP
