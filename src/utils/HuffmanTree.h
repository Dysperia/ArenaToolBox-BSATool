#ifndef BSATOOL_HUFFMANTREE_H
#define BSATOOL_HUFFMANTREE_H


#include <QtCore/QVector>
#include <array>
#include "BitsStreams.h"

using namespace std;

/**
 * @brief the tree is contained inside an array using the following pattern as example :
 *            6
 *          /   \
 *         4     5
 *        / \   / \
 *       0   1 2   3
 * In this example, the numbers are the index in the array. thus, mTree[4] = 0 and you can get children index from
 * mTree[4] and mTree[4]+1. As an other example, mTree[6] = 4.
 * The tree is not a complete binary tree so mTree contains all node from left to right one level after an other,
 * going up in the tree. As a result, the first nodes found are the leafs (314 exactly). One can know if there is a
 * right child by looking the value of the left child : if it is a leaf then there is no right child.
 *
 * Since there is no child for a leaf, the value is instead either a color index in a 256 color table or a length to copy (the first 256 are
 * colors and after lengths from 0 to 57 (actual lengths build from these values are in fact value+3 so lengths are in range
 * 3 - 60 in the compression algorithm).
 *
 * In order to isolate leaf values from node index, the leaf values are stored by adding 627. Since it is higher than
 * the maximum index in the tree, it cannot be misunderstood for an index and this test is used to know if a leaf is
 * found.
 *
 * To sum up : color index are leaf value minus 627 and length are leaf value minus 627 minus 256 plus 3.
 *
 * Since this is a Huffman tree, the frequency of seen values are kept and updated as the algorithm is running and the
 * tree is sorted to keep left frequency lower than right child's one.
 * A father frequency is the sum of the children's frequencies.
 */
class HuffmanTree {
private:
    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * Reset tree if the root frequency (equals total leaves frequencies) is too high, lowering frequencies in the
     * entire tree, possibly rearranging the nodes. the reset is triggered of total leaves frequency is 32768 or more.
     * Hence, the tree is reset after (32768-314) leaves are used
     */
    void resetTreeAtFreqTooHigh();

    /**
     * increase frequency of leaf by one and rearrange tree if needed to keep it ordred
     * @param leaf Leaf value from which to start
     */
    void increaseFreqLeaf(const quint16 &leaf);

    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * @brief this array contains the tree, the first 314 values being values to processed and the other the index of
     * the left child, the last value being the index of the tree root's left child.
     */
    array<quint16, 627> mTree{};
    /**
     * @brief this array contains a reverse tree allowing to find the index in mTree from a value (either leaf value
     * or node index). In the class example, mRevTree[4] would be 6. The value for the root (mRevTree[6] in the example
     * and mRevTree[626] in the real tree is 0, same value than for the first leaf. Since this array is used only while
     * climbing up the tree, it is ok, and zero means the root as been found.
     */
    array<quint16, 941> mRevTree{};
    /**
     * @brief this array contains the frequencies for the tree nodes. For a given index, mTree contains the left child
     * index and mFreq the frequency. So the left child frequency is mFreq[mTree[index]].
     * At the start, all leaves frequencies are 1 and nodes are just the sum of their children's frequencies. the root
     * has a initial frequency of 314 (the number of leaves).
     */
    array<quint16, 628> mFreq{};

public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * Initialized a tree, ready to be used for deflate compression/uncompression
     */
    HuffmanTree();

    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * Navigate the tree from the root using bits from given reader. It stops when a leaf is found. The found leaf's
     * frequency is increased by one, the tree resets if total frequency too high and the tree is processed to remained
     * ordered.
     * @param bitsReader Reader from which get the bits used to navigate the tree (0 = left child, 1 = right child)
     * @return the found leaf's unprocessed value
     */
    quint16 findLeaf(BitsReader &bitsReader);

    /**
     * Writing bits of the path from the root to the given leaf to the given writer. The leaf's
     * frequency is increased by one, the tree resets if total frequency too high and the tree is processed to remained
     * ordered.
     * @param bitsWriter Writer from which write the bits used to navigate the tree (0 = left child, 1 = right child)
     * @param leaf the leaf's unprocessed value
     */
    void writePathForLeaf(BitsWriter &bitsWriter, const quint16 &leaf);
};


#endif //BSATOOL_HUFFMANTREE_H
