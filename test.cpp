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
    
    CHECK((memcmp(a->getText(), s, sz) == 0));
    sz = strlen((char*)bwt);
    REQUIRE((memcmp(a->getBWT(), bwt, sz) == 0));
}

TEST_CASE("Short insert", "[short]"){
    ustring s = (ustring) "GAAGT";
    a->insertFactor(s, 3, 5);
    
    s = (ustring) "GTGAAGTGAGTGAG";
    ustring bwt = (ustring) "GGGGAATTT$AAGGG";
    int sz = strlen((char*)s);
    
    CHECK((memcmp(a->getText(), s, sz) == 0));
    sz = strlen((char*)bwt);
    REQUIRE((memcmp(a->getBWT(), bwt, sz) == 0));
}

TEST_CASE("Short delete", "[short]"){
    ustring s = (ustring) "GTGAGTGAG";
    ustring bwt = (ustring) "GGGATTA$GG";
    a -> deleteAt(3,5);
    
    int sz = strlen((char*)s);
    CHECK((memcmp(a->getText(), s, sz) == 0));
    sz = strlen((char*)bwt);
    REQUIRE((memcmp(a->getBWT(), bwt, sz) == 0));
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
    REQUIRE((memcmp(a -> getBWT(), bwt, sz) == 0));
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
    REQUIRE((memcmp(a -> getBWT(), bwt, sz) == 0));

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
    REQUIRE((memcmp(a -> getBWT(), bwt, sz) == 0));
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
/*
 * BWT for the example used in documentation
 */
TEST_CASE("Short setText1"," [short]") {
	ustring s=(ustring) "CTCTGC";
	ustring bwt=(ustring) "CG$TTCC";
	a = new dynsa::DynamicSuffixArray(fs);
	a->setText(s,7);
	int sz=strlen((char*)s);
	CHECK((memcmp(a->getText(),s,sz)==0));
	sz=strlen((char*)s);
	REQUIRE((memcmp(a->getBWT(),bwt,sz)==0));
}
TEST_CASE("Short insert1","[short]") {
	ustring s=(ustring) "G";
	a->insertFactor(s,3,1);
	ustring bwt = (ustring) "CGG$TTCC";

	s=(ustring) "CTGCTGC";
	int sz=strlen((char*)s);
	CHECK((memcmp(a->getText(),s,sz)==0));
	sz=strlen((char*)bwt);
	REQUIRE((memcmp(a->getBWT(),bwt,sz)==0));
}
TEST_CASE("524 into 3631  long bwt insert", "[insert]"){

    FILE* input = fopen("testdata/test6/GCA_000731455_3631.in", "r");
    ustring s1 = new uchar[3631];
    fscanf(input," %s", s1);
    fclose(input);

    FILE* insert = fopen("testdata/test6/GCA_000731455_524.in", "r");
    ustring s2 = new uchar[524];
    fscanf(insert," %s", s2);
    fclose(insert);

    int sz1 = strlen((char*)s1);
    int sz2 = strlen((char*)s2);


    FILE* bwt = fopen("testdata/test6/GCA_000731455_3631ins.out", "r");
    ustring s3 = new uchar[sz2+sz1];
    fscanf(insert," %s", s3);
    fclose(insert);



    a = new dynsa::DynamicSuffixArray(fs);
    a -> setText(s1, 3632);
    a->insertFactor(s2,500,524);
    REQUIRE((memcmp(a -> getBWT(), s3, sz1+sz2+1) == 0));
}
TEST_CASE("35970 delete - test7", "[delete]"){

    FILE* input = fopen("testdata/test7/GCA_000731455_35970.in", "r");
    ustring s = new uchar[35972];
    fscanf(input," %s", s);
    fclose(input);

    FILE* deleted = fopen("testdata/test7/GCA_000731455_35970del.in", "r");
    ustring del = new uchar[35972];
    fscanf(deleted," %s", del);
    fclose(deleted);
    int sz = strlen((char*)del);

    a = new dynsa::DynamicSuffixArray(fs);
    a -> setText(s, 35971);
    a ->deleteAt(4000,4000);

    CHECK((memcmp(a -> getText(), del, 31970) == 0));

    FILE* expected = fopen("testdata/test7/GCA_000731455_35970del", "r");
    ustring bwt = new uchar[31971];
    fscanf(expected," %s", bwt);
    fclose(expected);

    REQUIRE((memcmp(a -> getBWT(), bwt, 31971) == 0));
}
