#include "DynamicSuffixArray.h"

using namespace std;

int main() {
    dynsa::ustring s = new uchar[5];
    s[0] = (uchar) 'C';
    s[1] = (uchar) 'T';
    s[2] = (uchar) 'C';
    s[3] = (uchar) 'C';
    s[4] = (uchar) 0;

    float* fs = DynRankS::createCharDistribution(s, 5, 1);
    dynsa::DynamicSuffixArray * a = new dynsa::DynamicSuffixArray(fs);

    for(int i = 0; i < 5; i++) {
        cout << "Inserting " << s[i] << " " << i << endl;
        a->insertToText(s[i], i);
        cout << a->getText() << ", " << a->getBWT() << endl;
    }

    cout << a->getText() << endl;
    cout << a->getBWT() << endl;

    return 0;
}
