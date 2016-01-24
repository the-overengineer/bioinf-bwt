#include "DynamicSuffixArray.h"
#include <string.h>
#include <stdio.h>
#include <string>
#include <time.h>
#include <sys/time.h>
#include <fstream>

using namespace std;

dynsa::DynamicSuffixArray *a;
float *fs;

unsigned long long getTime() {
    struct timeval time;
    time_t sec;
    suseconds_t usec;
    if (gettimeofday(&time,NULL) != -1) {
        sec = time.tv_sec;
        usec = time.tv_usec;
        return (unsigned long long)sec*1000000+usec;
    }
    return 0;
}

int main( int argc, char* const argv[]){
    
    
    if (argc < 2 || argv[1][0] == 'h'){
        fprintf(stderr, "Usage: %s <filename> [insert_one <char> <position>] [insert <filename> <position> <string_length>] [delete <position> <length>] [replace <position> <filename> <string_length>] <output_filename>\n", argv[0]);
        exit(0);
    }
    
    FILE* input = fopen(argv[1], "r");
    ustring s = new uchar[1000000];
    fscanf(input," %s", s);
    int n = strlen((char*)s);
    fclose(input);
    
    fs = DynRankS::createCharDistribution(s, n, 1);
    a = new dynsa::DynamicSuffixArray(fs);
    
    a -> setText(s, n+1);
    
    ustring extra_input = new uchar[1000000];
    int position = -1;
    uchar extra_char = 0;
    int length = -1;
    
    
    int action = -1;
    if (argc > 3){
        if (argv[2][0] == 'd'){
            action = 0;
            position = atoi(argv[3]);
            length = atoi(argv[4]);
        }
        if (argv[2][0] == 'r'){
            action = 1;
            position = atoi(argv[3]);
            input = fopen(argv[4], "r");
            fscanf(input," %s", extra_input);
            fclose(input);
            length = atoi(argv[5]);
        }
        if (argv[2][0] == 'i' && argv[2][6] == '_'){
            action = 2;
            extra_char = (uchar) argv[3][0];
            position = atoi(argv[4]);            
        }
        if (argv[2][0] == 'i' && argv[2][6] == 0){
            action = 3;
            input = fopen(argv[3], "r");
            fscanf(input," %s", extra_input);
            fclose(input);
            position = atoi(argv[4]);   
            length = atoi(argv[5]);
        }
    }
    
    unsigned long long start = getTime();
    
    if (action == 0){
        a -> deleteAt(position, length);
    }
    if (action == 1){
        a -> replace(extra_input, position, length);
    }
    if (action == 2){
        a -> insertToText(extra_char, position);
    }
    if (action == 3){
        a -> insertFactor(extra_input, position, length);
    }
    
    unsigned long long end = getTime();
    cout << "Done in " << (end-start)/1000000. << "sec.   (Excluding the inicial text set)."<< endl;
    
    if (argc > 2){
        ofstream fout(argv[argc-1]);
        fout << a -> getBWT() << endl;
    }    
    
    return 0;
}

/*

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

TEST_CASE("Short replace", "[short]"){
    a -> replace((ustring) "TT", 3, 2);
    
    ustring s = (ustring) "GTTTGTGAG";
    ustring bwt = (ustring) "GGATT$GTTG";
    
    int sz = strlen((char*)s);
    CHECK((memcmp(a->getText(), s, sz) == 0));
    sz = strlen((char*)bwt);
    REQUIRE((memcmp(a->getBWT(), bwt, sz) == 0));
}

TEST_CASE("Single insert", "[short]"){
    a -> insertToText((uchar)'C', 4);
    ustring s = (ustring) "GTTCTGTGAG";
    ustring bwt = (ustring) "GGTATT$TGCG";
    
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
*/