#include "threetest.h"
#include "time.h"

ThreeTest::ThreeTest()
{
    srand(time(NULL));
}

RndTree* ThreeTest::CreateRndThree(int *count, Node *node, RndTree *pThree)
{

    if(*count <= 0)
        return nullptr;
    if(!node)
    {
        RndTree *three = new RndTree();
        three->Add("Root");
        node = three->FindNode("Root");
        CreateRndThree(count, node, three);
        return three;
    }
    int maxChildren = 10, currentChildrenCnt;
    currentChildrenCnt = rand() % maxChildren + 1;
    while (currentChildrenCnt--)
    {
        string name = to_string(*count);
        Node* pTmp = pThree->Add(name, node);
        (*count)--;
        if(*count <= 0)
            return nullptr;
        int tmpRnd = rand() % 2;
        if( tmpRnd)
           CreateRndThree(count, pTmp, pThree);
    }
    CreateRndThree(count, node, pThree);
    return nullptr;
}
