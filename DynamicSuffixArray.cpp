/**
 * ADD MIT license
 */

#include "DynamicSuffixArray.h"
#define FROM_LEAF(i) ((char) ((i) - C_DIM))
#define TO_LEAF(c) (C_DIM + (size_t) (c))
#define DUMP(x) std::cout << x << std::endl
#define MAX(a, b) ((a) > (b) ? (a) : (b))

namespace dynsa {

    DynamicSuffixArray::DynamicSuffixArray(float* factors) { 
        this->L = new DynRankS();
        this->initialize(factors);
        this->sample = new DSASampling(this, SAMPLE);
    }

    DynamicSuffixArray::~DynamicSuffixArray() {
        delete this->sample;
        delete this->L;
    }

    void DynamicSuffixArray::initialize(float* factors) {
        this->L->initEmptyDynRankS(factors);
        //Initialize the partial sums tree
        for(int i = 0; i < C_SIZE; i++) {
            this->C[i] = 0;
        }
    }

    void DynamicSuffixArray::insert(uchar c, size_t position) {
        this->L->insert(c, position);
        
        //Update the numbers in the tree, updating our partial sums that way
        size_t i = TO_LEAF(c);
        
        while(i > 1) {
            this->C[i]++;
            i = this->getParent(i);
        }

        this->C[i]++;
    }

    void DynamicSuffixArray::del(size_t position) {
        //Pop the symbol
        uchar c = this->L->deleteSymbol(position);

        //Update the partial sums
        size_t i = TO_LEAF(c);

        while(i > 1) {
            this->C[i]--;
            i = this->getParent(i);
        }

        this->C[i]--;
    }

    void DynamicSuffixArray::moveRow(size_t i, size_t j) {
        uchar c = this->getBWTAt(i);
        del(i);
        insert(c, j);
    }

    void DynamicSuffixArray::insertToText(uchar c, size_t position) {
        //position = MAX(position, this->size()); //Cannot insert after the end of the text

        //Step Ib modifies T^[position]
        DUMP("A"); 
        //a) Find the position via sampling
        size_t k = this->getISA(position);
        DUMP("B");
        //Stores the position of T^[position-1] for reordering later
        size_t previous_position = 0;

        //b) Perform the replacement, deleting the old char if there is one
        if(! this->isEmpty()) {
            uchar old_sym = this->getBWTAt(k);
            DUMP("D " << old_sym);
            previous_position = countSymbolsSmallerThan(old_sym) + rank(old_sym, k);
            DUMP("E " << previous_position);
            this->L->deleteSymbol(k);
            DUMP("F");
        }
        DUMP("G");
        insert(c, k); //Insert the replacement symbol
        DUMP("H");
        //Step 2a inserts a row @ LF(k)
        size_t insertion_point = this->countSymbolsSmallerThan(c) + this->rank(c, k);
        insert(c, insertion_point); //The Gonzales-Navarro structure should handle this
                                    //According to the paper, at least
                                  

        //Update our sampler. I honestly still have no idea what the sampler does
        sample->insertBWT(position, insertion_point);

        //Finally, step IIb, REORDER. We give it 
        //The parameters are j and index(T'^[i]), from which j' is computed
        reorder(previous_position, insertion_point);

        //Perform the final update of our sampler
        sample->insertBWT(position);
    }

    void DynamicSuffixArray::insertFactor(ustring s, size_t position, size_t length) {
        position = MAX(position, this->size());

        size_t pos_mod_bwt = 0; //TODO step Ib
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

        bwt[this->size()] = '\0'; //Delimit the string

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

        for(size_t i = N - 1, j = 1; i > 0; i--) {
            text[i - 1] = this->getBWTAt(j);
            j = this->LF(j);
        }

        return text;
    }


    size_t DynamicSuffixArray::rank(uchar c, size_t i) {
        return this->L->rank(c, i); //TODO special case handling during operations!
    }

    size_t DynamicSuffixArray::size() {
        return this->L->getSize();
    }

    bool DynamicSuffixArray::isEmpty() {
        return this->size() == 0;
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
                cnt += this->C[getLeftSubtree(parent)];    
            }

            i = parent;
        }


        //TODO handle deleting? How should we do that? Use a flag like in the original?
        return cnt;
    }


    size_t DynamicSuffixArray::getSA(size_t i) {
        return sample->getSA(i);
    }

    size_t DynamicSuffixArray::getISA(size_t i) {
        return sample->getISA(i);
    }

    size_t DynamicSuffixArray::LF(size_t i) {
        uchar c = this->getBWTAt(i);
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
                i -= smaller;
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

    void DynamicSuffixArray::reorder(size_t j, size_t insertion_point) {
        size_t j_comp = this->LF(insertion_point);
        size_t new_j = 0;

        while(j != j_comp) {
            new_j = this->LF(j);
            moveRow(j, j_comp);
            j = new_j;
            j_comp = this->LF(j_comp);
        }
    }
}
