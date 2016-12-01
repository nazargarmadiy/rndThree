#ifndef RNDTREE_H
#define RNDTREE_H

#include <string>
#include <vector>

using namespace std;

struct Node
{
    string value;
    vector<Node*> childs;
    Node(string _val);
};

class RndTree
{
private:
    Node* pRoot = nullptr;
    void GetCommand(Node* pHead, Node **pParrent, string *pCommand);
    Node* BuildFrame(string *command, Node **pCurr, Node **pParr);

public:
    RndTree();
    Node* Add(string val, Node* pDest = nullptr);
    string GetValue(Node* pNode);
    void PrintTree(Node* pHead = nullptr);
    Node* GetChild(Node* pParent, int num);
    Node* FindNode(string val, Node* pHead = nullptr);
    void PrettyPrint(Node* pHead = nullptr, int *level = nullptr, bool last = true);
    string GetCommand();
    Node* BuildTree(string command);
    ~RndTree();
};

#endif // RNDTREE_H
