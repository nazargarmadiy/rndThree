#ifndef RNDTREE_H
#define RNDTREE_H

#include <string>
#include <vector>

using namespace std;

#define COMMAND_SEPARATOR "\n"
#define SUB_COMMAND_SEPARATOR ";"
#define END_COMMANDS "end;"
/*
* comands must be represented only one ASCII sumbol!!!
*/
#define WRITE_NODE_COMMAND 'W'
#define UP_COMMAND 'U'
#define DOWN_COMMAND 'D'

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
    void Serialize(Node* pHead, Node **pParrent, string *pCommand);
    Node* BuildFrame(string *command, Node **pCurr, Node **pParr);

public:
    RndTree();
    Node* Add(string val, Node* pDest = nullptr);
    string GetValue(Node* pNode);
    Node* GetChild(Node* pParent, int num);
    Node* FindNode(string val, Node* pHead = nullptr);
    void PrettyPrint(Node* pHead = nullptr, int *level = nullptr, bool last = true);
    string Serialize();
    Node* Deserialize(string command);
    ~RndTree();
};

#endif // RNDTREE_H
