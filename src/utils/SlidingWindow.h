#ifndef BSATOOL_SLIDINGWINDOW_H
#define BSATOOL_SLIDINGWINDOW_H

#include <cstdlib>
#include <array>

using namespace std;

/**
 * The sliding window is a length fixed vue on the current uncompressed data. It should be moved after each
 * processed or read data. The value at the current insertion position is the data at the begining of the window and
 * the data before is the last read or processed.
 * The window cycles its current index position each _size, rewriting old data with new one, hence the sliding.
 * @tparam _type data type to store
 * @tparam _size total length of the window
 */
template<typename _type, size_t _size>
class SlidingWindow {
public:
    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    /**
     * return the index of the next insertion (index of the oldest data)
     * @return the index between 0 and _size - 1
     */
    [[nodiscard]] size_t getMCurrentInsertPosition() const;

    /**
     * return the array used to internally manage the window
     * @return an array of size _size and type _type
     */
    const array<_type, _size> &getWindow() const;

    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * Read the data at a given index in the window
     * @param index Index from which to read. If it is not in the range [0, _size-1] it will become using
     * modulo _size
     * @return the data at the given index
     */
    const _type &readAtIndex(const size_t &index) const;

    /**
     * Insert the given data at the current insertion index, replacing the oldest data. The current insertion index is
     * increased by one
     * @param newValue The value to insert
     */
    void insert(const _type &newValue);

    /**
     * Return the lowest positive index equivalent to the given one
     * @param index to get the standard from
     * @return the standard index
     */
    [[nodiscard]] size_t getStandardEquivalentIndex(const size_t &index) const;

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * Index of the oldest data, the next to be replace
     */
    size_t mCurrentInsertPosition = 0;
    /**
     * Array used to internally build the sliding window
     */
    array<_type, _size> mWindow{};
};

#include "SlidingWindow.cpp"

#endif // BSATOOL_SLIDINGWINDOW_H
