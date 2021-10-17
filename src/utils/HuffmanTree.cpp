#include <utils/HuffmanTree.h>

using namespace std;

//******************************************************************************
// Constructors
//******************************************************************************
HuffmanTree::HuffmanTree() {
    // Initializing leaves
    for (quint16 i(0); i < 314; i++) {
        mFreq[i] = 1;
        mTree[i] = 627 + i;
        mRevTree[i + 627] = i;
    }
    // initializing other nodes
    quint16 mFreqIdx = 0;
    for (quint16 i(314); i <= 626; i++) {
        mFreq[i] = mFreq[mFreqIdx] + mFreq[mFreqIdx + 1];
        mTree[i] = mFreqIdx;
        mRevTree[mFreqIdx + 1] = i;
        mRevTree[mFreqIdx] = i;
        mFreqIdx += 2;
    }
    mFreq[627] = 0xFFFF;
    // Root as no parent -> 0
    mRevTree[626] = 0;
}

//**************************************************************************
// Methods
//**************************************************************************
void HuffmanTree::resetTreeAtFreqTooHigh() {
    // Reset because total freq too high on root
    if (mFreq[626] == 0x8000) {
        // gathering leaf at the beginning and halving their freq
        quint16 nextLeafFreeIndex = 0;
        for (quint16 currentNode(0); currentNode < 627; currentNode++) {
            // if leaf
            if (mTree[currentNode] >= 627) {
                mFreq[nextLeafFreeIndex] = (mFreq[currentNode] + 1) / 2;
                mTree[nextLeafFreeIndex] = mTree[currentNode];
                nextLeafFreeIndex++;
            }
        }

        // for other nodes, rebuilding remaining tree
        quint16 currentLeftChild = 0;
        for (quint16 currentNode(314); currentNode < 627; currentNode++) {
            // current node freq is sum of children
            mFreq[currentNode] = mFreq[currentLeftChild] + mFreq[currentLeftChild + 1];
            quint16 currentNodeFreq = mFreq[currentNode];
            // currentNode has been lowered in freq -> searching superior freq before in case tree broken
            quint16 farthestPreviousWithHigherFreq = currentNode - 1;
            while (mFreq[farthestPreviousWithHigherFreq] > currentNodeFreq) {
                farthestPreviousWithHigherFreq--;
            }
            farthestPreviousWithHigherFreq++;
            // swaping node to keep tree organized if needed and setting up current parent
            quint16 nbBytesToSwap = (currentNode - farthestPreviousWithHigherFreq) * 2;
            memmove(&mFreq[farthestPreviousWithHigherFreq + 1], &mFreq[farthestPreviousWithHigherFreq], nbBytesToSwap);
            mFreq[farthestPreviousWithHigherFreq] = currentNodeFreq;
            memmove(&mTree[farthestPreviousWithHigherFreq + 1], &mTree[farthestPreviousWithHigherFreq], nbBytesToSwap);
            mTree[farthestPreviousWithHigherFreq] = currentLeftChild;
            // moving to next children
            currentLeftChild += 2;
        }

        // rebuilding reverse tree
        for (quint16 currentNode(0); currentNode < 627; currentNode++) {
            quint16 leftChild = mTree[currentNode];
            if (leftChild < 627) {
                mRevTree[leftChild + 1] = currentNode;
            }
            mRevTree[leftChild] = currentNode;
        }
    }
}

void HuffmanTree::increaseFreqLeaf(const quint16 &leaf) {
    // keeping tree ordered while increasing freq
    quint16 currentNode = mRevTree[leaf];
    do {
        mFreq[currentNode] += 1;
        quint16 currentNodeFreq = mFreq[currentNode];
        quint16 LastNodeWithLowerFreq = currentNode + 1;
        // if current node was the last with that frequency : nothing to do
        if (mFreq[LastNodeWithLowerFreq] < currentNodeFreq) {
            // searching last node with old value
            while (mFreq[LastNodeWithLowerFreq] < currentNodeFreq) {
                LastNodeWithLowerFreq++;
            }
            LastNodeWithLowerFreq--;
            // switching nodes and their subtrees
            // nodes freq
            mFreq[currentNode] = mFreq[LastNodeWithLowerFreq];
            mFreq[LastNodeWithLowerFreq] = currentNodeFreq;
            // subtrees parent reference in reverse tree
            quint16 currentNodeLeftChild = mTree[currentNode];
            mRevTree[currentNodeLeftChild] = LastNodeWithLowerFreq;
            // if not a leaf, there is a right child to update
            if (currentNodeLeftChild < 627) {
                mRevTree[currentNodeLeftChild + 1] = LastNodeWithLowerFreq;
            }
            quint16 lastNodeWithLowerFreqLeftChild = mTree[LastNodeWithLowerFreq];
            mRevTree[lastNodeWithLowerFreqLeftChild] = currentNode;
            // if not a leaf, there is a right child to update
            if (lastNodeWithLowerFreqLeftChild < 627) {
                mRevTree[lastNodeWithLowerFreqLeftChild + 1] = currentNode;
            }
            // subtrees parent reference in main tree
            mTree[LastNodeWithLowerFreq] = currentNodeLeftChild;
            mTree[currentNode] = lastNodeWithLowerFreqLeftChild;
            currentNode = LastNodeWithLowerFreq;
        }
        // getting parent node and starting again up to the root
        currentNode = mRevTree[currentNode];
    } while (currentNode != 0);
}

quint16 HuffmanTree::findLeaf(BitsReader &bitsReader) {
    // searching leaf in tree from input
    quint16 leaf = mTree[626];
    while (leaf < 627) {
        quint8 bits = bitsReader.getBits();
        bitsReader.removeBits(1);
        quint16 childChoice = bits >> 7u;
        leaf = mTree[leaf + childChoice];
    }
    resetTreeAtFreqTooHigh();
    increaseFreqLeaf(leaf);
    return leaf;
}

void HuffmanTree::writePathForLeaf(BitsWriter &bitsWriter, const quint16 &leaf) {
    deque<quint16> path;
    quint16 node = mRevTree[leaf];
    quint8 left = 0;
    quint8 right = 0x80u;
    while (node < 626) {
        quint16 parent = mRevTree[node];
        path.push_front(mTree[parent] == node ? left : right);
        node = parent;
    }
    for (auto direction : path) {
        bitsWriter.addBits(direction, 1u);
    }
    resetTreeAtFreqTooHigh();
    increaseFreqLeaf(leaf);
}
