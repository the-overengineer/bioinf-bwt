#include "DynamicSuffixArray.h"

using namespace std;

int main() {
    dynsa::ustring s = new uchar[5];
    s[0] = (uchar) 'G';
    s[1] = (uchar) 'T';
    s[2] = (uchar) 'G';
    s[3] = (uchar) 'A';
    s[4] = (uchar) 0;

    float* fs = DynRankS::createCharDistribution(s, 6, 1);
    dynsa::DynamicSuffixArray * a = new dynsa::DynamicSuffixArray(fs);

    a->insertToText('A', 1);
    a->insertFactor(s, 1, 5);

    cout << s << endl;
    cout << a->getText() << endl;
    cout << a->getBWT() << endl;

    return 0;
}
