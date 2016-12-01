#ifndef THREETEST_H
#define THREETEST_H
#include <rndtree.h>

using namespace std;

class ThreeTest
{
public:
    ThreeTest();
    RndTree* CreateRndThree(int *count, Node* node = nullptr, RndTree* pThree = nullptr);
};

#endif // THREETEST_H
