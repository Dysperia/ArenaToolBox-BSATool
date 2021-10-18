//**************************************************************************
// Getters/setters
//**************************************************************************
template<typename _type, size_t _size>
size_t SlidingWindow<_type, _size>::getMCurrentInsertPosition() const {
    return mCurrentInsertPosition;
}

template<typename _type, size_t _size>
const array<_type, _size> &SlidingWindow<_type, _size>::getWindow() const {
    return mWindow;
}

//**************************************************************************
// Methods
//**************************************************************************
template<typename _type, size_t _size>
const _type &SlidingWindow<_type, _size>::readAtIndex(const size_t &index) const {
    return mWindow[getStandardEquivalentIndex(index)];
}

template<typename _type, size_t _size>
void SlidingWindow<_type, _size>::insert(const _type &newValue) {
    mWindow[mCurrentInsertPosition] = newValue;
    mCurrentInsertPosition = (mCurrentInsertPosition + 1) % _size;
}

template<typename _type, size_t _size>
size_t SlidingWindow<_type, _size>::getStandardEquivalentIndex(const size_t &index) const {
    return index % _size;
}

template<typename _type, size_t _size>
typename SlidingWindow<_type, _size>::DuplicateSearchResult SlidingWindow<_type, _size>::searchDuplicateInSlidingWindowLookAheadOnly(
        const deque<_type> &uncompressDataDeque, const size_t max_duplicate_length) {
    // search longest possible considering max duplicate length and remaining uncompressed data
    quint8 max_possible_duplicate_length(
            uncompressDataDeque.size() < max_duplicate_length ? uncompressDataDeque.size() : max_duplicate_length);
    // building preview window using current data and future one
    QVector<char> snapshotFutureWindow;
    // end of current buffer
    for (int i(-max_duplicate_length); i < 0; ++i) {
        snapshotFutureWindow.push_back(readAtIndex(getMCurrentInsertPosition() + i));
    }
    // data that will next be written in buffer
    for (size_t i(0); i < max_possible_duplicate_length - 1; ++i) {
        snapshotFutureWindow.push_back(uncompressDataDeque[i]);
    }
    // searching for duplicate
    DuplicateSearchResult result = {0, 0};
    const char &nextUncompressedByte = uncompressDataDeque.front();
    for (int i(0); i < max_duplicate_length && result.length < max_possible_duplicate_length; ++i) {
        // found start for a match
        if (nextUncompressedByte == snapshotFutureWindow[i]) {
            // computing sequence length
            quint8 tempLength(1);
            // computing while not the longest length and available data
            while (tempLength < max_possible_duplicate_length &&
                   snapshotFutureWindow[i + tempLength] == uncompressDataDeque[tempLength]) {
                tempLength++;
            }
            // writing result if longer
            if (tempLength > result.length) {
                result.length = tempLength;
                result.startIndex = getStandardEquivalentIndex(getMCurrentInsertPosition() + i - max_duplicate_length);
            }
        }
    }
    return result;
}

template<typename _type, size_t _size>
typename SlidingWindow<_type, _size>::DuplicateSearchResult SlidingWindow<_type, _size>::searchDuplicateInSlidingWindowNoLookAhead(
        const deque<_type> &uncompressDataDeque, const size_t max_duplicate_length) {
    DuplicateSearchResult result = {0, 0};
    quint8 tempLength;
    quint16 tempStartIndex;
    // searching a first byte match until longest found or all window searched
    // starting at offset 1 from current position to avoid the window current index
    const char &nextUncompressedByte = uncompressDataDeque.front();
    for (int i = 1; i < 4096 - max_duplicate_length && result.length < max_duplicate_length; ++i) {
        tempStartIndex = getStandardEquivalentIndex(getMCurrentInsertPosition() + i);
        // Found a possible match
        if (nextUncompressedByte == readAtIndex(tempStartIndex)) {
            tempLength = 1;
            // computing length for the found match, while checking if enough data available for it
            while (tempLength < uncompressDataDeque.size() &&
                   tempLength < max_duplicate_length &&
                   uncompressDataDeque[tempLength] == readAtIndex(tempStartIndex + tempLength)) {
                tempLength++;
            }
            // keeping only if longer than a previous one
            if (tempLength > result.length) {
                result.length = tempLength;
                result.startIndex = tempStartIndex;
            }
        }
    }
    return result;
}

template<typename _type, size_t _size>
typename SlidingWindow<_type, _size>::DuplicateSearchResult SlidingWindow<_type, _size>::searchDuplicateInSlidingWindow(const deque<_type> &uncompressDataDeque,
                                                                               const size_t max_duplicate_length) {
    // searching for an ongoing duplicate using the possibly rewritten part of the window
    const DuplicateSearchResult lookAhead = searchDuplicateInSlidingWindowLookAheadOnly(uncompressDataDeque,
                                                                                         max_duplicate_length);
    DuplicateSearchResult noLookAhead = {0, 0};
    // not longest found
    if (lookAhead.length < max_duplicate_length) {
        // Search through buffer in case there is a longer duplicate to copy avoiding the possibly
        // rewritten section already search before
        noLookAhead = searchDuplicateInSlidingWindowNoLookAhead(uncompressDataDeque, max_duplicate_length);
    }
    return lookAhead.length > noLookAhead.length ? lookAhead : noLookAhead;
}
