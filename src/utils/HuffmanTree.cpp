#include "HuffmanTree.h"

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
    if (mFreq[0x0272] == 0x8000) {
        // sub_31_60D start
        quint16 parentIdx = 0x0000;
        quint16 freqValue;
        quint16 local3;
        quint16 idx;
        quint16 childIdx = 0x0000;
        // for the entire tree
        while (childIdx < 0x0273) {
            // if not leaf
            if (mTree[childIdx] < 0x0273) {
                mFreq[parentIdx] = (mFreq[childIdx] + 1u) >> 1u;
                mTree[parentIdx] = mTree[childIdx];
                parentIdx++;
            }
            childIdx++;
        }

        childIdx = 0x0000;
        parentIdx = 0x013A;
        // on parent or right sibling node
        while (parentIdx < 0x0273) {
            idx = childIdx + 1;
            mFreq[parentIdx] = mFreq[childIdx] + mFreq[idx];
            freqValue = mFreq[parentIdx];

            idx = parentIdx - 1;
            while (mFreq[idx] > freqValue) {
                idx--;
            }
            idx++;
            local3 = (parentIdx - idx);
            // sub_00_7718 start
            // sub_00_7736 start
            if (local3 < 1) {
                for (quint16 i(0); i < local3; i++) {
                    mFreq[idx + 1 + local3 - 1 - i] = mFreq[idx + local3 - 1 - i];
                }
            } else {
                for (quint16 i(0); i < (local3 >> 2u); i++) {
                    mFreq[idx + 1 + i * 2] = mFreq[idx + i * 2];
                    mFreq[idx + 1 + i * 2 + 1] = mFreq[idx + i * 2 + 1];
                }
                for (quint16 i(0); i < (local3 & 0x0003u); i++) {
                    mFreq[idx + 1 + i + (local3 >> 2u)] = mFreq[idx + i + (local3 >> 2u)];
                }
            }
            // sub_00_7736 end
            // sub_00_7718 end
            mFreq[idx] = freqValue;
            // sub_00_7718 start
            // sub_00_7736 start
            if (local3 < 2) {
                for (quint16 i(0); i < local3; i++) {
                    mTree[idx + 1 + local3 - 1 - i] = mTree[idx + local3 - 1 - i];
                }
            } else {
                for (quint16 i(0); i < (local3 >> 2u); i++) {
                    mTree[idx + 1 + i * 2] = mTree[idx + i * 2];
                }
                for (quint16 i(0); i < (local3 & 0x0003u); i++) {
                    mTree[idx + 1 + i] = mTree[idx + i];
                }
            }
            // sub_00_7736 end
            // sub_00_7718 end
            mTree[idx] = childIdx;
            childIdx += 2;
            parentIdx++;
        }
        // rebuilding reverse tree
        childIdx = 0x0000;
        while (childIdx < 0x0273) {
            idx = mTree[childIdx];
            if (idx < 0x0273) {
                mRevTree[idx + 1] = childIdx;
            }
            mRevTree[idx] = childIdx;
            childIdx++;
        }
        // sub_31_60D end
    }
}

void HuffmanTree::increaseFreqLeaf(const quint16 &leaf) {
// keeping tree ordered while increasing freq
    quint16 parentNodeIdx = mRevTree[leaf];
    do {
        mFreq[parentNodeIdx] += 1;
        quint16 parentNodeFreq = mFreq[parentNodeIdx];
        quint16 nextNodeIdx = parentNodeIdx + 1;
        if (mFreq[nextNodeIdx] < parentNodeFreq) {
            while (mFreq[nextNodeIdx] < parentNodeFreq) {
                nextNodeIdx++;
            }
            nextNodeIdx--;
            mFreq[parentNodeIdx] = mFreq[nextNodeIdx];
            mFreq[nextNodeIdx] = parentNodeFreq;
            quint16 leftChildIdx = mTree[parentNodeIdx];
            mRevTree[leftChildIdx] = nextNodeIdx;
            // if not a leaf
            if (leftChildIdx < 627) {
                mRevTree[leftChildIdx + 1] = nextNodeIdx;
            }
            quint16 nextNodeLeftChild = mTree[nextNodeIdx];
            mTree[nextNodeIdx] = leftChildIdx;
            mRevTree[nextNodeLeftChild] = parentNodeIdx;
            if (nextNodeLeftChild < 627) {
                mRevTree[nextNodeLeftChild + 1] = parentNodeIdx;
            }
            mTree[parentNodeIdx] = nextNodeLeftChild;
            parentNodeIdx = nextNodeIdx;
        }
        parentNodeIdx = mRevTree[parentNodeIdx];
    } while (parentNodeIdx != 0);
}

quint16 HuffmanTree::findLeaf(BitsReader &bitsReader) {
    // searching leaf in tree from input
    quint16 leaf = mTree[626];
    while (leaf < 627) {
        quint16 bits = bitsReader.getBits();
        bitsReader.removeBits(1);
        quint16 childChoice = bits >> 15u;
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
