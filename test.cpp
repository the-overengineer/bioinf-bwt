#include "DynamicSuffixArray.h"

using namespace std;

int main() {
    dynsa::ustring s = new uchar[8];
    s[0] = (uchar) 'G';
    s[1] = (uchar) 'G';
    s[2] = (uchar) 'C';
    s[3] = (uchar) 0;
    s[4] = (uchar) 'A';
    s[5] = (uchar) 'A';
    s[6] = (uchar) 'C';
    s[7] = (uchar) 'C';

    float* fs = DynRankS::createCharDistribution(s, 8, 1);
    dynsa::DynamicSuffixArray * a = new dynsa::DynamicSuffixArray(fs);

    for(int i = 0; i < 8; i++) {
        a->insert(s[i], i + 1);
        cout << a->getText() << ", " << a->getBWT() << endl;
    }

    cout << a->getText() << endl;
    cout << a->getBWT() << endl;

    return 0;
}
