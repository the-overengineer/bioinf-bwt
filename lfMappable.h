#ifndef LF_MAP
#define LF_MAP

#include "types.h"

namespace dynsa {
    class LFMap {
        public:

        virtual size_t LF(size_t i) = 0;

        virtual size_t FL(size_t i) = 0;
    };
}

#endif
