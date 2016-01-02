/**
 * ADD MIT license
 */

#include "DynamicSuffixArray.h"
#define C_DIM 256
#define C_SIZE (C_DIM + C_DIM)
#define FROM_LEAF(i) ((char) ((i) - C_DIM))
#define TO_LEAF(c) (C_DIM + (size_t) (c))

namespace dynsa {

    DynamicSuffixArray::DynamicSuffixArray(bool computeLCP) {
        //Initialize the C array/tree
        for(int i = 0; i < C_SIZE; i++) {
            this->C[i] = 0;
        }

        this->lcp = new LCP(this, computeLCP);
        this->L = new DynRankS();
    }

    DynamicSuffixArray::~DynamicSuffixArray() {
        delete this->lcp;
        delete this->L;
    }

    void DynamicSuffixArray::insert(uchar c, size_t position) {
        L->insert(c, position);

        //Update the numbers in the tree
        size_t i = TO_LEAF(c);

        while(i > 1) {
            C[i]++;
            i = this->getParent(i);
        }

        C[i]++;
    }

    void DynamicSuffixArray::insertFactor(ustring s, size_t position, size_t length) {
        //TODO insertFactor
    }

    void DynamicSuffixArray::deleteAt(size_t position) {
        //TODO deleteAt
    }

    void DynamicSuffixArray::replace(ustring s, size_t position, size_t size) {
        //TODO replace
    }

    ustring DynamicSuffixArray::getBWT() {
        //One extra for the '$'
        ustring bwt = new uchar[this->size() + 1];

        uchar c; //Temporary storage for a character
        size_t i = 0; //The current index in text

        this->L->iterateReset(); //Reset the iterator

        while(true) {
            //Check for $ as a special character
            c = this->L->iterateGetSymbol();
            bwt[i++] = c == '\0' ? '$' : c;

            //Break if done
            if(! this->L->iterateNext()) {
                break;
            }
        }

        bwt[i - 1] = '\0'; //Delimit the string

        return bwt;
    }

    uchar DynamicSuffixArray::getBWTAt(size_t i) {
        return (*L)[i];
    }

    ustring DynamicSuffixArray::getText() {
        size_t N = this->size();
        ustring text = new uchar[N];
        
        //TODO what about fetching the text during substitution?
        // Should not matter, but check
        
        //Set the EOS
        text[N - 1] = '\0';

        for(size_t i = N - 2, j = 1; i >= 0; i--) {
            text[i] = this[j];
            j = this->LF(j);
        }

        return text;
    }

    uchar DynamicSuffixArray::operator[] (size_t i) {
        return this->getBWTAt(i); //Shorthand
    }

    size_t DynamicSuffixArray::rank(uchar c, size_t i) {
        return this->L->rank(c, i); //TODO special case handling during operations!
    }

    size_t DynamicSuffixArray::size() {
        return this->L->getSize();
    }

    /*
     * Private methods! Possibly also dragons.
     */

    size_t DynamicSuffixArray::countSymbolsSmallerThan(uchar c) {
        //Start from the character's leaf in the tree
        size_t i = TO_LEAF(c);
        size_t cnt = 0;

        //As long as there can be left subtrees (smaller counts in them), go up
        while(i > 1) {
            size_t parent = getParent(i);

            //If this is the right subtree, everything in the left
            //sibling is smaller!
            if(this->isRightSubtree(i)) {
                cnt += getLeftSubtree(parent);    
            }

            i = parent;
        }

        //TODO handle deleting? How should we do that? Use a flag like in the original?
        
        return cnt;
    }

    size_t DynamicSuffixArray::LF(size_t i) {
        uchar c = this[i];
        //TODO special cases while modifying
        return this->countSymbolsSmallerThan(c) + this->rank(c, i);
    }

    size_t DynamicSuffixArray::FL(size_t i) {
        uchar c; //i = getRankInF(i, c)

        size_t smaller, node = 1; //Start at root

        //Descend down the tree to locate the character
        //Correct the cumulative sum while descending right
        while(node < C_DIM) {
            smaller = C[getLeftSubtree(node)];

            if(smaller >= i) {
                node = getLeftSubtree(node);
            } else {
                node = getRightSubtree(node);
                i -= l;
            }
        }

        c = FROM_LEAF(node); //Get char to which the leaf node belongs

        return this->L->select(c, i);
    }

    bool DynamicSuffixArray::isRightSubtree(size_t i) {
        return i % 2 == 1;
    }

    bool DynamicSuffixArray::isLeftSubtree(size_t i) {
        return i % 2 == 0;
    }

    size_t DynamicSuffixArray::getLeftSubtree(size_t i) {
        return 2 * i;
    }

    size_t DynamicSuffixArray::getRightSubtree(size_t i) {
        return 2 * i + 1;
    }

    size_t DynamicSuffixArray::getParent(size_t i) {
        return floor(i / 2);
    }

    void DynamicSuffixArray::reorder() {
        return; //TODO implement
    }
}
