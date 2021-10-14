#ifndef BSATOOL_PALETTE_H
#define BSATOOL_PALETTE_H

#include <QRgb>
#include <QVector>

/**
 * @brief Describe the palette format
 *
 * The file describes a palette according to the following.
 * The header (not present in integrated palette) is composed of these information :
 * - Data total size (always 776 : 8 bytes header + 768 color bytes) : 4 bytes
 * - Major version (always 0xb123) : 2 bytes
 * - Minor version (always 0x0000) : 2 bytes
 * The header information is not stored in this class because useless.
 * Then follows the data :
 * - 256 colors each encoded as 3 color bytes in the following order : red green blue
 * The first color is considered fully transparent if needed as there is no alpha information stored
 */
class Palette
{
public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * @brief Palette constructor. Build a default 256 colors gray palette
     * with colors from (R=0, G=0, B=0, A=255) to (R=255, G=255, B=255, A=255)
     */
    Palette();
    /**
     * @brief Palette constructor. Extract a palette from the given data
     * @param rgbs palette description given as 256 RGB triplet (hence 768 length vector)
     * @param sixBitsColor colors are expected to be coded on 6 bits if true, 8 bits otherwise. False by default
     */
    explicit Palette(const QVector<char> &rgbs, bool sixBitsColor = false);
    /**
     * @brief Palette constructor. Extract a palette from the given file
     * @param filePath to the palette description file
     * @param sixBitsColor colors are expected to be coded on 6 bits if true, 8 bits otherwise. False by default
     */
    explicit Palette(const QString &filePath, bool sixBitsColor = false);

    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    [[nodiscard]] QVector<QRgb> getColorTable() const;

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    QVector<QRgb> mColorTable{};

    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * Read the palette data from the given stream
     * enough data
     * @param imgDataStream stream from which read the data
     * @param sixBitsColor true if color are encoded with 6 bits, false if 8bits
     * @throw Status if stream size not at least 768
     */
    void readDataFromStream(QDataStream &imgDataStream, bool sixBitsColor);
};

#endif // BSATOOL_PALETTE_H
