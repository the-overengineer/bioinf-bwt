/**
 * ADD MIT license
 */

#include "DynamicSuffixArray.h"
#define FROM_LEAF(i) ((char) ((i) - C_DIM))
#define TO_LEAF(c) (C_DIM + (size_t) (c))
#define DUMP(x) std::cout << x << std::endl
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

namespace dynsa {

    DynamicSuffixArray::DynamicSuffixArray(float* factors) { 
        this->operation = none;
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
        if(operation == inserting || operation == deleting || operation == replacing) {
            if(position <= previous_position) {
                previous_position++;
            }

            if(position <= first_modification_position) {
                first_modification_position++;
            }
        }

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

        if(operation == inserting || operation == deleting || operation == replacing) {
            if(previous_position >= position) {
                previous_position--;
            }

            if(first_modification_position >= position) {
                first_modification_position--;
            }
        }
    }

    void DynamicSuffixArray::moveRow(size_t i, size_t j) {
        uchar c = this->getBWTAt(i);
        del(i);
        insert(c, j);
    }

    void DynamicSuffixArray::insertToText(uchar c, size_t position) {
        //Cannot insert after the end of text
        position = MIN(position, this->size());

        if(this->isEmpty()) {
            this->insert(c, 1);
            sample->insertBWT(1, 1);
            sample->insertBWT(1);
            return;
        }

        //Step Ib modifies T^[position]
        //a) Find the position via sampling
        k = this->getISA(position);
       
        //Stores the position of T^[position-1] for reordering later
        uchar old_sym;

        //b) Perform the replacement, deleting the old char if there is one
        if(! this->isEmpty()) {
            old_sym = this->getBWTAt(k);
            previous_position = countSymbolsSmallerThan(old_sym) + rank(old_sym, k);
            this->L->deleteSymbol(k);
        }

        //Try a fix?! FIXME
        //if(old_sym > c) {
        //    previous_position--;
        //}
        
        insert(c, k); //Insert the replacement symbol
      
        new_sym = c;
        this->operation = inserting;

        //if(old_sym > new_sym) {
        //    previous_position--;
        //}

        //Step IIa inserts a row @ LF(k)
        size_t insertion_point = this->countSymbolsSmallerThan(c) + this->rank(c, k);
       
        //if(pos_first_modif < k && c == old_sym) {
        //    insertion_point++;
        //}

        insert(old_sym, insertion_point); //The Gonzales-Navarro structure should handle this
                                    //According to the paper, at least
        
        //if(insertion_point <= previous_position) {
        //
        //}

        //Update our sampler. I honestly still have no idea what the sampler does
        sample->insertBWT(position, insertion_point);

        //

        //Finally, step IIb, REORDER. We give it 
        //The parameters are j and index(T'^[i]), from which j' is computed
        //reorder(previous_position, insertion_point);

        //Perform the final update of our sampler
        sample->insertBWT(position);
    }

    void DynamicSuffixArray::insertFactor(ustring s, size_t position, size_t length) {
        position = MIN(position, this->size());

        //Step Ib modifies T^[position]
        //a) Find the position via sampling
        size_t pos_in_bwt = this->getISA(position);
        size_t rank_of_deleted = 0;

        //b) Perform the replacement, deleting the old char if there is one
        if(! this->isEmpty()) {
            old_sym = this->getBWTAt(pos_in_bwt);
            rank_of_deleted = rank(old_sym, pos_in_bwt);
            this->L->deleteSymbol(pos_in_bwt);
        }
       
        //The last character of the string
        uchar c = s[length - 1];

        this->insert(c, pos_in_bwt); //Insert the replacement symbol
        first_modification_position = pos_in_bwt; //Update our first modification position in this run

        insertion_point = pos_in_bwt;
        this->new_sym = c;
        this->operation = inserting;

        previous_position = this->countSymbolsSmallerThan(old_sym) + rank_of_deleted;

        if(old_sym > new_sym) {
            previous_position--;
        }

        size_t old_insertion_point = insertion_point;

        //Step IIa inserts a bunch of rows
        insertion_point = this->countSymbolsSmallerThan(c) + this->rank(c, insertion_point);

        for(size_t j = length - 1; j > 0; j--) {
           this->insert(s[j -1], insertion_point);
           modifications_remaining = k;
           new_sym = s[j - 1];

           sample->insertBWT(position, insertion_point);
           old_insertion_point = insertion_point;

           insertion_point = this->countSymbolsSmallerThan(s[j - 1]) + this->rank(s[j - 1], insertion_point);

           if(first_modification_position < old_insertion_point && s[j - 1] == old_sym) {
             insertion_point++;
           }
        }

        new_sym = old_sym;
        L->insert(old_sym, insertion_point);
        modifications_remaining = 0;

        if(insertion_point <= previous_position) {
            previous_position++;
        }


        if(insertion_point <= first_modification_position) {
            first_modification_position++;
        }

        //Update our sampler. I honestly still have no idea what the sampler does
        sample->insertBWT(position, insertion_point);

        //Finally, step IIb, REORDER. We give it 
        reorder();

        //Perform the final update of our sampler
        sample->insertBWT(position);
    }

    void DynamicSuffixArray::deleteAt(size_t position, size_t length) {
        if(length <= 0) {
            return; //Nothing to do here
        }

        size_t end_position = position + length - 1; //The end of the deleted block
        size_t rank_of_deleted;

        uchar last_symbol;

        //Make sure we do not delete the '\0' or anything outside of the string
        if(end_position > this->size()) {
            length = this->size() - position + 1;
        }

        //Sample the ISA for the last character in the substring
        first_modification_position = this->getISA(length + position);
        old_sym = this->getBWTAt(first_modification_position);

        //Notify the object that we are deleting (for various off-by-one error resolving)
        operation = deleting;

        insertion_point = this->countSymbolsSmallerThan(old_sym) + rank(old_sym, first_modification_position);

        //Actually delete the substring
        for(size_t i = length + position - 1; i >= position; i--) {
            last_symbol = this->getBWTAt(insertion_point);
            rank_of_deleted = this->rank(last_symbol, insertion_point);
            
            this->del(insertion_point);
            this->sample->deleteBWT(i, insertion_point);
            
            if(i == position) {
                break;
            }

            insertion_point = this->countSymbolsSmallerThan(last_symbol) + rank_of_deleted;
        }

        previous_position = this->countSymbolsSmallerThan(last_symbol) + rank_of_deleted;
        
        insertion_point = first_modification_position;
        this->del(insertion_point);
        this->new_sym = last_symbol;
        insert(last_symbol, insertion_point);

        reorder();
        
        //Perform a final sampler update
        this->sample->deleteBWT(position);
    }

    void DynamicSuffixArray::replace(ustring s, size_t position, size_t length) {
        //TODO replace
    }

    void DynamicSuffixArray::setText(ustring s, size_t size) {
        this->insert(s[size - 1], 1);
        sample->insertBWT(1, 1);
        sample->insertBWT(1);
        this->insertFactor(s, 1, size - 1);
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
        
        for(size_t i = N - 1, j = 1; i > 0; i--) {
            text[i - 1] = this->getBWTAt(j);
            if(operation == inserting && j == insertion_point) {
                text[i - 1] = old_sym;
            }

            j = this->LF(j);
        }

        //Set the EOS
        text[N - 1] = '\0';


        return text;
    }


    size_t DynamicSuffixArray::rank(uchar c, size_t i) {
        size_t r = this->L->rank(c, i);

        if(operation == deleting && first_modification_position < i && old_sym == c) {
            r--;
        }

        return r;
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

        /** 
         * We have not yet deleted the symbol from the tree, so we must move any
         * characters that would appear afterwards in F backwards one spot.
         */
        return (operation == deleting && c > old_sym) ? cnt - 1 : cnt;
    }


    size_t DynamicSuffixArray::getSA(size_t i) {
        return sample->getSA(i);
    }

    size_t DynamicSuffixArray::getISA(size_t i) {
        if(this->isEmpty()) {
            return 1;
        }

        return sample->getISA(i);
    }

    size_t DynamicSuffixArray::LF(size_t i) {
        if(operation != none && i == insertion_point) {
            return previous_position;
        }

        uchar c = this->getBWTAt(i);
        //TODO special cases while modifying
        size_t smaller = this->countSymbolsSmallerThan(c);
        size_t r = this->rank(c, i);

        if(operation != none) {
            if(operation == reordering || (operation == inserting && modifications_remaining == 0)) {
                uchar c2 = this->getBWTAt(insertion_point);
                size_t tempLF = this->countSymbolsSmallerThan(c2) + this->rank(c2, insertion_point);
                size_t result = smaller + r;
                
                if(result <= tempLF && result >= previous_position) {
                    result++;
                } else if(result >= tempLF && result <= previous_position) {
                    result--;
                }

                return result;
            } else if(operation == inserting) {
                uchar other = this->getBWTAt(insertion_point);
                if(other == old_sym && i > first_modification_position) {
                    r++;
                }

                if(c > new_sym) {
                    smaller--;
                }

                if(other == c && i > insertion_point) {
                    r--;
                }
            }

        }
        
        return smaller + r;
    }

    size_t DynamicSuffixArray::FL(size_t i) {
        uchar c;

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

    void DynamicSuffixArray::reorder() {
        operation = reordering;
        uchar L_store = new_sym;
        size_t expected_position = this->countSymbolsSmallerThan(L_store) + this->rank(L_store, insertion_point);

        size_t smaller = this->countSymbolsSmallerThan(L_store);

        while(expected_position != previous_position) {
            DUMP("x " << previous_position);
            L_store = this->getBWTAt(previous_position);
            smaller = this->countSymbolsSmallerThan(L_store);

            size_t tmp_prev_pos = rank(L_store, previous_position) + smaller;

            this->del(previous_position);
            this->insert(L_store, expected_position);

            sample->moveBWT(previous_position, expected_position);

            insertion_point = expected_position;
            previous_position = tmp_prev_pos;
            expected_position = smaller + rank(L_store, insertion_point);
        }

        operation = none;
        insertion_point = expected_position;
    }
}
