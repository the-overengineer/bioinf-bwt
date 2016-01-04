#ifndef DYN_SUFF_ARR

#define DYN_SUFF_ARR

#include "types.h"
#include <math.h>
#include "gRankS.h"
#include "sbvtree.h"
#include "intTree.h"
#include "DSASampling.h"
#include "lfMappable.h"

#define C_DIM 256
#define C_SIZE ((C_DIM) * (C_DIM))

//Taken from the paper's code
#define SAMPLE 42

using namespace sbvtree;

namespace dynsa {

    typedef uchar* ustring;
    
    class DynamicSuffixArray : public LFMap {
        public:

        /**
         * Constructs a new suffix array.
         */
        DynamicSuffixArray(float* factors);

        /**
         * Performs suffix array destruction
         */
        ~DynamicSuffixArray();

        void initialize(float* factors);

        /**
         * Inserts a character c into L (BWT) at the given position
         * and updates the partial sums inside C.
         *
         * @param <uchar> c - Character to insert into text
         * @param <size_t> position - Position at which to insert
         */
        void insert(uchar c, size_t position);

        /**
         * Removes a character at given position in L and updates
         * the partial sums.
         *
         * @param <size_t> position - Position at which the deleting
         *                            happening.
         */
        void del(size_t position);


        /**
         * Moves a row from position i to position j.
         *
         * @param <size_t> i - Origin row
         * @param <size_t> j - Destination row
         */
        void moveRow(size_t i, size_t j)

        void insertToText(uchar c, size_t position);

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

        /**
         * Checks whether the SA is empty
         *
         * @return <bool> - Whether SA is empty
         */
        bool isEmpty();

        /**
         * Computes LF mapping (from last column to first) of index
         *
         * @param <size_t> i - Position in L
         * @return <size_t> - Position i F
         */
        size_t LF(size_t i);

         /*
         * Computes the FL mapping, the inverse of the LF mapping,
         * of an index.
         *
         * @param <size_t> i - Position in F
         * @return <size_t> - Position in L
         */
         size_t FL(size_t i);


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
         * The sampler user for SA and ISA mappings
         */
        DSASampling* sample;

        /**
         * Returns the number of symbols smaller than a given symbol.
         * //TODO elaborate
         *
         * @param <uchar> c - Character for which we want a smaller count
         * @return <size_t> - Number of symbols smaller than a given symbol
         */
        size_t countSymbolsSmallerThan(uchar c);

        /**
         * Get suffix array mapping for index i
         *
         * @param <size_t> i - Index
         * @return <size_t> - Index in SA
         */
        size_t getSA(size_t i);

        /**
         * Get inverse suffix array mapping for index i
         *
         * @param <size_t> i - Index
         * @return <size_t> - Index in ISA
         */
        size_t getISA(size_t i);

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
         * Performs the REORDER step (IIb) of the DynSA algorithm.
         * In this case <j> is the index of T^[i-1], while insertion_point
         * is the point at which we inserted our new row.
         *
         * Might have to be corrected when other operations are added, TODO
         */
        void reorder(size_t j, size_t insertion_point);
    };
};

#endif
