#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>
#include <QRgb>
#include <QPixmap>
#include <QVector>
#include <map>
#include "bsafile.hpp"

class Image
{
private:

    // Set to true if last build image had transparency
    bool hasTransparency;

    // Image offset X
    uint16_t offsetX;

    // Image offset Y
    uint16_t offsetY;

    // Image width
    uint16_t width;

    // Image height
    uint16_t height;

    // Image flags
    uint16_t flags;

    // Image data size
    uint16_t dataSize;

    // Decompressed image size (present only if (flags & 0x00FF) = 0x0008)
    uint16_t decompressedSize;

    // Map containing all the palettes used for image building (with first color not transparent)
    std::map<std::string, QVector<QRgb>> palettesMap;

    // Map containing all the palettes used for image building (with first color transparent)
    std::map<std::string, QVector<QRgb>> transparentPalettesMap;

    // Image buffer containing image raw data
    uchar *imageData;

    // Image buffer containing compressed image data
    uchar *compressedImageData;

    // Structure for table of native Arena IMG file and their palette
    struct natIMG
    {
        std::string fileName;
        std::string paletteName;
        std::string paletteType;
    };

    // Table of native Arena IMG file and their palette
    natIMG nativeIMGsList[947];

    // Structure for table of native Arena SET file and their dimensions
    struct natSET
    {
        std::string fileName;
        int width;
        int height;
    };

    // Table of native Arena SET file and their dimensions
    natSET nativeSETsList[180];

    // Structure for table of native Arena CIF files and their palette
    struct natCIF
    {
        std::string fileName;
        std::string paletteName;
    };

    // Table of native Arena CIF files and their palette
    natCIF nativeCIFsList[59];

    // structure for table of image with no header and dimensions to set manually
    struct noH
    {
        std::string fileName;
        int width;
        int height;
    };

    // Table of IMG with no header and dimensions to set manually
    noH noHeaderIMG[15];

    // Table of CIF file with raw data and dimension to set manually
    noH noHeaderCIF[6];

    // Map of IMG/COL filename and their respective palette filename (if COL file or IMG with integrated palette)
    std::map<std::string, std::string> nativePaletteMap;

    // List of all native IMG(with integrated palette)/COL file
    QStringList nativePaletteFileList;

    // Image
    QImage Img;

    // Name of palette used to build the last QImage
    std::string palette;

public:

    // Structure to store single frame objects and variables
    struct frameData
    {
        // Set to true if frame use transparency
        bool hasTransparency;

        // Some objects used to store frame data
        QImage imageQImage;
        std::string imagePaletteName;
        QVector<QRgb> imagePalette;
    };

private:

    // Object to store multiple image frame data when building animation
    std::vector<frameData> animationData;

public:

    // Constructor
    Image();

    // Destructor
    ~Image();

    // Build a QImage from file index and return it
    QImage buildQImage(int index, std::string extension);

    // Build a QImage from file path and return it
    // Overloaded function
    QImage buildQImage(const std::string filePath);

    // Build animation from filepath (if different of "") or index
    std::vector<frameData> buildAnimation(int index, const std::string filePath = "");

    // Extract and decompress an IMG/SET file
    int extractDecompressImage(int index, const std::string &filePath);

    // Extract, decompress and convert an IMG/SET file to png
    int extractDecompressConvertImage(int index, const std::string &filePath);

    // Decompress external IMG
    int decompressExternalIMG(const std::string &fileName, const std::string &filePath);

    // Return image flags
    uint16_t getImageFlags();

    // Return image width
    uint16_t getImageWidth();

    // Return image height
    uint16_t getImageHeight();

    // Return true if filename is one of the native IMG
    bool getIsNativeIMG(const std::string &fileName);

    // Return true if filename is one of the native SET
    bool getIsNativeSET(const std::string &fileName);

    // return width of the SET image if native or 0 if not
    int getNativeSETWidth(const std::string &fileName);

    // return height of the SET image if native or 0 if not
    int getNativeSETHeight(const std::string &fileName);

    // Return palette type (texture/noHeader/0/1/8/9) if filename is one of the native IMG, empty string otherwise
    std::string getNativeIMGPaletteType(const std::string &fileName);

    // Return name of the palette used to build the last QImage
    std::string getPaletteName();

    // Return palette used to build the last QImage
    QVector<QRgb> getPalette(const std::string &palette, bool transparency = false);

    // Return transparency value for the last built QImage
    bool getTransparency();

    // Return Arena native palette file list
    QStringList getPaletteFileList();

    // Return noHeaderIMG.width or 0 if image is not in the list
    int getNoHeaderIMGWidth(const std::string &fileName);

    // Return noHeaderIMG.height or 0 if image is not in the list
    int getNoHeaderIMGHeight(const std::string &fileName);
};

#endif // IMAGE_HPP
