#include "DynamicSuffixArray.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <time.h>
#include <sys/time.h>

using namespace std;

/*int main() {
    dynsa::ustring s = new uchar[5];
    s[0] = (uchar) 'G';
    s[1] = (uchar) 'T';
    s[2] = (uchar) 'G';
    s[3] = (uchar) 'A';
    s[4] = (uchar) 0;

    float* fs = DynRankS::createCharDistribution(s, 5, 1);
    dynsa::DynamicSuffixArray * a = new dynsa::DynamicSuffixArray(fs);
    a->setText(s, 5);

    cout << a->getText() << endl;
    cout << a->getBWT() << endl;

    a->deleteAt(1, 2);

    cout << a->getText() << endl;
    cout << a->getBWT() << endl;

    a->deleteAt(1, 1);

    cout << a->getText() << endl;
    cout << a->getBWT() << endl;

    return 0;
}
*/

ullong getTime();

TEST_CASE("One equals one", "[one]"){
    ullong start = getTime();
    dynsa::ustring s = new uchar[10];
    s[0] = (uchar) 'G';
    s[1] = (uchar) 'T';
    s[2] = (uchar) 'G';
    s[3] = (uchar) 'A';
    s[4] = (uchar) 'G';
    s[5] = (uchar) 'T';
    s[6] = (uchar) 'G';
    s[7] = (uchar) 'A';
    s[8] = (uchar) 'G';
    s[9] = (uchar) 0;

    float* fs = DynRankS::createCharDistribution(s, 10, 1);
    dynsa::DynamicSuffixArray * a = new dynsa::DynamicSuffixArray(fs);
    a->setText(s, 10);

    cout << a->getText() << endl;
    cout << a->getBWT() << endl;
    ullong stop = getTime();
    cout << stop - start << endl;
    REQUIRE(1==1);
}

TEST_CASE("Zero equals zero", "[zero]"){
    REQUIRE( 0 == 0 );
}

ullong getTime() {
    struct timeval time;
    time_t sec;
    suseconds_t usec;
    if (gettimeofday(&time,NULL) != -1) {
        sec = time.tv_sec;
        usec = time.tv_usec;
        return (ullong)sec*1000000+usec;
    }
}