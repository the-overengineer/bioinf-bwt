#ifndef DYN_SUFF_ARR

#define DYN_SUFF_ARR

#include "types.h"
#include <math.h>
#include "gRankS.h"
#include "sbvtree.h"
#include "intTree.h"
#include "lcp.h"

#define C_DIM 256
#define C_SIZE ((C_DIM) * (C_DIM))

// Sampling?! WTF?

using namespace sbvtree;

namespace dynsa {

    typedef ustring uchar*;
    
    class DynamicSuffixArray {
        public:

        /**
         * Constructs a new suffix array.
         * Optionally computes the LCP table to speed things up.
         * NOTE: LCP is not yet implemented.
         *
         * @param <bool> computeLCP - Whether the LCP table is computed
         */
        DynamicSuffixArray(bool computeLCP = false);

        /**
         * Performs suffix array destruction
         */
        ~DynamicSuffixArray();

        /**
         * Inserts a character c into the text T at the given position.
         *
         * @param <uchar> c - Character to insert into text
         * @param <size_t> position - Position at which to insert
         */
        void insert(uchar c, size_t position);

        /**
         * Inserts a factor (string) of given size into the text at
         * some position.
         *
         * @param <ustring> s - String to insert into text
         * @param <size_t> position - Position at which the insertion is happening
         * @param <size_t> length - The length of the string inserted
         */
        void insertFactor(ustring s, size_t position, size_t length);

        /**
         * Deletes the character at a given position in the text.
         *
         * @param <size_t> position - Position of deleted character in text
         */
        void deleteAt(size_t position);

        /**
         * Performs replacement of a substring in the text.
         * Text from given position onwards is replaced with a string
         * of given length.
         *
         * @param <ustring> s - Replacement string
         * @param <size_t> position - Position from which the replacement starts
         * @param <size_t> size - Size of the replacement substring, and, as such
         *                        the count of chars replaced by the operation
         */
        void replace(ustring s, size_t position, size_t size);

        /**
         * Yields the current BWT of the text
         *
         * @return <ustring> - The BWT of the text
         */
        ustring getBWT();

        /**
         * Yields the BWT character at given position.
         *
         * @param <size_t> i - Index of character
         * @return <uchar> - Character at given position in BWT
         */
        uchar getBWTAt(size_t i);

        /**
         * Yields the original text
         *
         * @return <ustring> - The text
         */
        ustring getText();

        /**
         * Yields a character at a given position in the BWT.
         * Shorthand for getBWTAt
         *
         * @param <size_t> i - The index of character in BWT
         * @return <uchar> - Character at given position in BWT
         */
        uchar operator[] (size_t i);

        /**
         * Computes the rank of a given character at position i.
         *
         * @param <uchar> c - Character for which to compute rank
         * @param <size_t> i - Position at which the rank is computed
         * @return <size_t> - The rank for given parameters
         */
        size_t rank(uchar c, size_t i);


        /**
         * Yields the size of the text (and as such its BWT).
         *
         * @return <size_t> - The size of the text
         */
        size_t size();

        private:

        /**
         * The C_t matrix as a tree wrapped in an array.
         */
        size_t C[C_SIZE];

        /**
         * The last column of the text; Also the BWT
         */
        DynRankS* L;

        /**
         * Returns the number of symbols smaller than a given symbol.
         * //TODO elaborate
         *
         * @param <uchar> c - Character for which we want a smaller count
         * @return <size_t> - Number of symbols smaller than a given symbol
         */
        size_t countSymbolsSmallerThan(uchar c);


        /**
         * Computes LF mapping (from last column to first) of index
         *
         * @param <size_t> i - Position in L
         * @return <size_t> - Position i F
         */
        size_t LF(size_t i);

        /**
         * Computes the FL mapping, the inverse of the LF mapping,
         * of an index.
         *
         * @param <size_t> i - Position in F
         * @return <size_t> - Position in L
         */
        size_t FL(size_t i);

        /**
         * Checks whether the subtree is a right subtree
         * by its index in an array representation of a tree.
         */
        bool isRightSubtree(size_t i);

        /**
         * Checks whether the subtree is a left subtree by
         * its index in an array representation of a tree.
         */
        bool isLeftSubtree(size_t i);

        /**
         * Yields the index of a left subtree of a given tree node.
         */
        size_t getLeftSubtree(size_t i);

        /**
         * Yields the index of a right subtree of a given tree node.
         */
        size_t getRightSubtree(size_t i);

        /**
         * Yields the index of a parent node of a given tree node.
         */
        size_t getParent(size_t i);

        /**
         * Performs the REORDER step (IIb) of the DynSA algorithm
         */
        void reorder();
    }
}

#endif
