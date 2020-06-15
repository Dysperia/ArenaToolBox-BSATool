#ifndef BSATOOL_BITSSTREAMS_H
#define BSATOOL_BITSSTREAMS_H

#include <deque>
#include <QVector>

using namespace std;

/**
 * @brief Number of bits in a byte
 */
const quint8 NB_BITS_IN_BYTE = 8;

/**
 * Store bits in a 16 bits buffer
 */
struct BitsBuffer {
    /**
     * Actual usable bits in mBits. If mRemainingBits equals 10, only the 10 highest bits are values, the 6 next are
     * garbage
     */
    quint8 mRemainingBits = 0;
    /**
     * bits stored. Can store up to 16 bits. If not 16, the bits to be used are the high ones. leftmost bits are the
     * ones read first from the source. The bits are to be used from high to low. As an example, if 9 bits are usable,
     * mBits is as follow (bx being unused) :
     * b1 - b2 - b3 - b4 - b5 - b6 - b7 - b8 - b9 - bx - bx - bx - bx - bx - bx - bx
     */
    quint16 mBits = 0;
};

/**
 * Helper class to read bits from a source
 */
class BitsReader {
private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * Source from which read bits
     */
    deque<char> &mSource;
    /**
     * Bits buffer
     */
    BitsBuffer mBitsBuffer{0, 0};

public:
    //**************************************************************************
    // Statics
    //**************************************************************************
    /**
     * @brief Number of bits in a byte
     */
    const static quint8 NB_BITS_IN_BYTE = 8;
    /**
     * Extract the front byte of the deque as an unsigned byte and remove it from the deque
     * @param source data from which extract the front byte
     * @return the next unsigned byte
     */
    static uchar getNextUnsignedByte(deque<char> &source);

    /**
     * Extract the front byte of the deque and remove it from the deque
     * @param source data from which extract the front byte
     * @return the next byte
     */
    static char getNextByte(deque<char> &source);

    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * construct a reader from the given source
     * @param source source from which read the bits
     */
    explicit BitsReader(deque<char> &source);
    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * tell the reader that the nbBitsToRemove highest bits have been used
     * @param nbBitsToRemove
     */
    void removeBits(const quint8 &nbBitsToRemove);

    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    /**
     * Return the number of usable bits. Only use after a call to getBits(), not before, as getting bits can trigger
     * the retrieval of new bits from the source and change the number of usable bits
     * @return
     */
    [[nodiscard]] quint8 getRemainingBits() const;
    /**
     * Get new bits from the source if needed and return bits. This method ensure that at least the 8 highest bits are
     * usable. Refer to getRemainingBits() to know how many bits are usable
     * @return the bits
     */
    quint16 getBits();
};

/**
 * Helper class to write bits to a destination deque
 */
class BitsWriter {
private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * Destination to which write bits
     */
    QVector<char> &mDestination;
    /**
     * Bits buffer
     */
    BitsBuffer mBitsBuffer{0, 0};

    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * Write the bits currently contained in the buffer's high byte
     */
    void flushHighByte();

public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * construct a reader from the given source
     * @param destination destination to which write the bits
     */
    explicit BitsWriter(QVector<char> &destination);

    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * Add bits and write bits if needed to avoid an overflow
     * @param byte bits to write
     * @param nbBits number of bits usable in byte
     */
    void addBits(quint8 byte, quint8 nbBits);

    /**
     * Write the bits currently contained in the buffer
     */
    void flush();
};


#endif //BSATOOL_BITSSTREAMS_H
