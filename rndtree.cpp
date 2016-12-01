#include "rndtree.h"

Node::Node(string _val)
{
    value = _val;
    children = vector<Node*>();
}

RndTree::RndTree(){}

RndTree::~RndTree()
{
    if(!pRoot)
        pRoot->children.clear();
}

Node* RndTree::Add(string val, Node *pDest)
{
    Node *newNode = new Node(val);
    if(pDest)
        pDest->children.push_back(newNode);
    else
    {
        if(pRoot)
            newNode->children.push_back(pRoot);
        pRoot = newNode;
    }
    return newNode;
}

string RndTree::GetValue(Node *pNode)
{
    return pNode->value;
}

Node* RndTree::GetChild(Node *pParent, int num)
{
    if(!pRoot)
        return nullptr;
    if(!pParent && pRoot->children.size() > num)
        return pParent->children[num];
    return nullptr;
}

Node* RndTree::FindNode(string val, Node *pHead)
{
    if(!pRoot)
        return nullptr;
    if(!pHead)
        pHead = pRoot;
    if(!pHead->value.compare(val))
        return pHead;
    Node* pFind = nullptr;
    vector<Node*>::iterator it = pHead->children.begin();
    while (it != pHead->children.end() && !pFind)
    {
        pFind = FindNode(val, *it);
        it++;
    }
    return pFind;
}


void RndTree::Serialize(Node *pHead, string *pCommand)
{
    if(!pHead)
        return;
    pCommand->append(string(1, WRITE_NODE_COMMAND));
    pCommand->append(":");
    pCommand->append(pHead->value);
    pCommand->append(SUB_COMMAND_SEPARATOR);
    for(int i = 0; i < pHead->children.size(); i++)
    {
        pCommand->append(string(1, DOWN_COMMAND));
        pCommand->append(SUB_COMMAND_SEPARATOR);
        pCommand->append(COMMAND_SEPARATOR);
        Serialize(pHead->children[i], pCommand);
        pCommand->append(string(1, UP_COMMAND));
        pCommand->append(COMMAND_SEPARATOR);
    }
}

string RndTree::Serialize()
{
    string* command = new string();
    if(!pRoot)
        return *command;
    Serialize(pRoot, command);
    command->append(END_COMMAND);
    return *command;
}

Node* RndTree::BuildFrame(string *command, Node **pCurr, Node **pParr)
{
    Node* pRoot;

        while(command->compare(END_COMMAND))
        {
            int index = command->find_first_of(COMMAND_SEPARATOR);
            string subCommand = command->substr(0, index);
            command->erase(0, index + 1);
            char commandSym = subCommand[0];
            switch (commandSym)
            {
                case WRITE_NODE_COMMAND://write new node
                {
                    subCommand.erase(0, 2);
                    int tmpIndex = subCommand.find(SUB_COMMAND_SEPARATOR);
                    string nodeValue =subCommand.substr(0, tmpIndex);

                    Node* pNew = new Node(nodeValue);
                    pRoot = pNew;
                    *pCurr = pNew;

                    subCommand.erase(0, tmpIndex + 1);
                    commandSym = subCommand[0];
                    if(commandSym == DOWN_COMMAND)//get deeper
                    {
                        pNew->children.push_back(nullptr);
                        BuildFrame(command, &(pNew->children.back()), &pNew);
                        break;
                    }
                    else if(commandSym == UP_COMMAND)//go to the upper level
                    {
                        return nullptr;
                    }
                }
                case DOWN_COMMAND://go to child
                {
                    (*pCurr)->children.push_back(nullptr);
                    BuildFrame(command, &(*pCurr)->children.back(), pCurr);
                    break;
                }
                case UP_COMMAND://return to upper level
                {
                    return nullptr;
                }
                default://unknown kommand
                    break;
            }
        }
        return pRoot;
}

Node* RndTree::Deserialize(string command)
{
    Node *pHead;
    return pRoot = BuildFrame(&command, &pHead, &pHead);
}

void RndTree::PrettyPrint(Node *pHead, int *level, bool last)
{
    if(!pHead && !pRoot)
    {
        printf("Tree is empty.\n");
        return;
    }
    if(!pHead)
    {
        int level = 0;
        PrettyPrint(pRoot, &level, true);
        return;
    }
    string prefiks;
    for(int i = 0; i < *level; i++)
    {
        if(i == *level -1)
            prefiks.append("└╴");
        else
            prefiks.append("|  ");
    }
    printf("%s>%s\n",prefiks.c_str(), pHead->value.c_str());
    (*level)++;
    vector<Node*>::iterator it = pHead->children.begin();
    while (it != pHead->children.end())
    {
        bool bTmp = (it == (pHead->children.end() - 1));
        PrettyPrint(*it, level, bTmp);
        it++;
    }
    (*level)--;
}

