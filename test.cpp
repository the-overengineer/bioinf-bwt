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


//Common variables for some tests
dynsa::DynamicSuffixArray *a;
ustring dist_string;
float *fs;

int main( int argc, char* const argv[]){
    
    dist_string = (ustring) "CCGAACACCCGTACCGAGGTGAGCACCCTCTGCATCAGCCAGTTCCGCTACAGCGCACAAATCCGTCCTTCNNNNNNNNNNNNNNNNNNNTTCCGTAGTGACCAAAGACTACACCTTTAAACGCCCCGGCTGGGCCGGACGTTTTGAACAGGAAGGCCAGCACCAGGACTACCAGCGCACGCAGTATGAAGTGTATGACTACCCCGGACGTTTCAAGAGCGCCCACGGGCAGAACTTTGCCCGCTGGCAGATGGACGGCTGGCGAAACAACGCAGAAACCGCGCGGGGAATGAGCCGCTCGCCGGAGATATGGCCGGGACGGCGAATTGTGCTGACGGGGCATCCGCAGGCGAACCTGAACCGGGAATGGCAGGTGGTGGCAAGTGAACTGCACGGCGAACAGCCACAGGCGGTGCCAGGACGGCAGGGAGCGGGGACGGCGCTGGAGAACCATTTTGCGGTGATCCCGGCAGACAGAACATGGCGACCACAGCCGTTGCTGAAACCGCTGGTCGACGGCCCGCAGAGCGC";

    fs = DynRankS::createCharDistribution(dist_string, 512, 1);
    a = new dynsa::DynamicSuffixArray(fs);
    
    int result = Catch::Session().run(argc, argv);
    
    return result;
}

TEST_CASE("Short setText", "[short]"){
    ustring s = (ustring) "GTGAGTGAG";
    ustring bwt = (ustring) "GGGATTA$GG";
    a->setText(s, 10);
    int sz = strlen((char*)s);
    
    CHECK( memcmp(a->getText(), s, sz) == 0);
    sz = strlen((char*)bwt);
    REQUIRE(memcmp(a->getBWT(), bwt, sz) == 0);
}

TEST_CASE("Short insert", "[short]"){
    ustring s = (ustring) "GAAGT";
    a->insertFactor(s, 3, 5);
    
    s = (ustring) "GTGAAGTGAGTGAG";
    ustring bwt = (ustring) "GGGGAATTT$AAGGG";
    int sz = strlen((char*)s);
    
    CHECK( memcmp(a->getText(), s, sz) == 0);
    sz = strlen((char*)bwt);
    REQUIRE(memcmp(a->getBWT(), bwt, sz) == 0);
}

TEST_CASE("Short delete", "[short]"){
    ustring s = (ustring) "GTGAGTGAG";
    ustring bwt = (ustring) "GGGATTA$GG";
    a -> deleteAt(3,5);
    
    int sz = strlen((char*)s);
    CHECK( memcmp(a->getText(), s, sz) == 0);
    sz = strlen((char*)bwt);
    REQUIRE(memcmp(a->getBWT(), bwt, sz) == 0);
}

TEST_CASE("1031 long bwt verification - test1", "[verification]"){
    
    FILE* input = fopen("testdata/test1/GCA_000731455_1031.in", "r");
    ustring s = new uchar[1033];
    fscanf(input," %s", s);
    fclose(input);
    
    FILE* expected = fopen("testdata/test1/GCA_000731455_1031.out", "r");
    ustring bwt = new uchar[1033];
    fscanf(expected," %s", bwt);
    fclose(expected);
    int sz = strlen((char*)bwt);
    
    a = new dynsa::DynamicSuffixArray(fs);
    a -> setText(s, 1032);
    REQUIRE(memcmp(a -> getBWT(), bwt, sz) == 0);
}

TEST_CASE("5385 long bwt verification - test2", "[verification]"){
    
    FILE* input = fopen("testdata/test2/GCA_000731455_5385.in", "r");
    ustring s = new uchar[5388];
    fscanf(input," %s", s);
    fclose(input);
    
    FILE* expected = fopen("testdata/test2/GCA_000731455_5385.out", "r");
    ustring bwt = new uchar[5388];
    fscanf(expected," %s", bwt);
    fclose(expected);
    int sz = strlen((char*)bwt);
    
    a = new dynsa::DynamicSuffixArray(fs);
    a -> setText(s, 5386);
    REQUIRE(memcmp(a -> getBWT(), bwt, sz) == 0);
}

TEST_CASE("35970 long bwt verification - test3", "[verification]"){
    
    FILE* input = fopen("testdata/test3/GCA_000731455_35970.in", "r");
    ustring s = new uchar[35972];
    fscanf(input," %s", s);
    fclose(input);
    
    FILE* expected = fopen("testdata/test3/GCA_000731455_35970.out", "r");
    ustring bwt = new uchar[35972];
    fscanf(expected," %s", bwt);
    fclose(expected);
    int sz = strlen((char*)bwt);
    
    a = new dynsa::DynamicSuffixArray(fs);
    a -> setText(s, 35971);
    REQUIRE(memcmp(a -> getBWT(), bwt, sz) == 0);
}

TEST_CASE("176616 large time monitor - test4", "[time]"){
    
    FILE* input = fopen("testdata/test4/GCA_000731455_176616.in", "r");
    ustring s = new uchar[176620];
    fscanf(input," %s", s);
    fclose(input);
    
    a = new dynsa::DynamicSuffixArray(fs);
    int sz = strlen((char*)s);
    a -> setText(s, 176617);
    REQUIRE(memcmp(a -> getText(), s, sz) == 0);
}

TEST_CASE("Check size after modifications", "[size]"){
    
    FILE* input = fopen("testdata/test5/GCA_000731455_189561.in", "r");
    ustring s = new uchar[189565];
    fscanf(input," %s", s);
    fclose(input);
    
    a = new dynsa::DynamicSuffixArray(fs);
    int sz = strlen((char*)s);
    CHECK( a -> size() == 0);
    CHECK( a -> isEmpty() == true);
    a -> setText(s, 148);
    CHECK( a -> size() == 148);
    a -> insertFactor(s+1200, 37, 231);
    CHECK( a -> size() == 379);
    a -> deleteAt(47, 85);
    CHECK( a -> size() == 294);
    a -> deleteAt(1,280);
    CHECK( a -> size() == 14);
    a -> deleteAt(1,12);
    CHECK( a -> isEmpty() == false);
    REQUIRE( a -> size() == 2);
}