/**
 * ADD MIT license
 */

#include "DynamicSuffixArray.h"

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
        //TODO insert
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
        //Add an additional space for the null character
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
        return NULL; //TODO
    }

    ustring DynamicSuffixArray::getTextFactor(size_t start, size_t length) {
        return NULL; //TODO
    }

    uchar DynamicSuffixArray::operator[] (size_t i) {
        return '$'; //TODO
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
        return 0; //TODO implement
    }

    size_t DynamicSuffixArray::LF(size_t i) {
        return i;
    }

    size_t DynamicSuffixArray::FL(size_t i) {
        return i;
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
