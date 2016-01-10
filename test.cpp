#include "DynamicSuffixArray.h"
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include <string.h>
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

dynsa::DynamicSuffixArray *a;

int main( int argc, char* const argv[]){
    
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
    a = new dynsa::DynamicSuffixArray(fs);
    
    int result = Catch::Session().run(argc, argv);
    
    return result;
}

TEST_CASE("setText sets short text", "setTextShort"){
    
    
    ustring s = (ustring) "GTGAGTGAG";
    ustring bwt = (ustring) "GGGATTA$GG";
    a->setText(s, 10);
    int sz = sizeof(s);
    
    CHECK( memcmp(a->getText(), s, sz) == 0);
    sz = sizeof(bwt);
    REQUIRE(memcmp(a->getBWT(), bwt, sz) == 0);
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
    return 0;
}