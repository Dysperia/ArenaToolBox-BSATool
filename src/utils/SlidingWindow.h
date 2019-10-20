#ifndef BSATOOL_SLIDINGWINDOW_H
#define BSATOOL_SLIDINGWINDOW_H

#include <cstdlib>
#include <array>

using namespace std;

template<typename _type, size_t _size>
class SlidingWindow {
public:
    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    size_t getMCurrentInsertPosition() const;
    void setMCurrentInsertPosition(size_t mCurrentInsertPosition);

    const array<_type, _size> &getWindow() const;
    void setWindow(const array<_type, _size> &window);

    //**************************************************************************
    // Methods
    //**************************************************************************
    const _type &readAtIndex(const size_t &index) const;
    void insert(const _type &newValue);

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    size_t mCurrentInsertPosition = 0;
    array<_type, _size> mWindow{};
};

#include "SlidingWindow.cpp"

#endif // BSATOOL_SLIDINGWINDOW_H
