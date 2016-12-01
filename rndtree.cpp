#include "rndtree.h"

Node::Node(string _val)
{
    value = _val;
    childs = vector<Node*>();
}

RndTree::RndTree(){}

RndTree::~RndTree()
{
    if(!pRoot)
        pRoot->childs.clear();
}

Node* RndTree::Add(string val, Node *pDest)
{
    Node *newNode = new Node(val);
    if(pDest)
        pDest->childs.push_back(newNode);
    else
    {
        if(pRoot)
            newNode->childs.push_back(pRoot);
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
    if(!pParent && pRoot->childs.size() > num)
        return pParent->childs[num];
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
    vector<Node*>::iterator it = pHead->childs.begin();
    while (it != pHead->childs.end() && !pFind)
    {
        pFind = FindNode(val, *it);
        it++;
    }
    return pFind;
}


void RndTree::Serialize(Node *pHead, Node **pParrent, string *pCommand)
{
    if(!pHead)
        return;
    pCommand->append(string(1, WRITE_NODE_COMMAND));
    pCommand->append(":");
    pCommand->append(pHead->value);
    pCommand->append(";");
    for(int i = 0; i < pHead->childs.size(); i++)
    {
        pCommand->append(string(1, DOWN_COMMAND));
        pCommand->append(";");
        pCommand->append(COMMAND_SEPARATOR);
        Serialize(pHead->childs[i], nullptr, pCommand);
        pCommand->append(string(1, UP_COMMAND));
        pCommand->append(COMMAND_SEPARATOR);
    }
}

string RndTree::Serialize()
{
    string* command = new string();
    if(!pRoot)
        return *command;
    Serialize(pRoot, &pRoot, command);
    command->append(END_COMMANDS);
    return *command;
}

Node* RndTree::BuildFrame(string *command, Node **pCurr, Node **pParr)
{
    Node* pRoot;
        while(command->compare(END_COMMANDS))
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
                    int tmpIndex = subCommand.find(";");
                    string nodeValue =subCommand.substr(0, tmpIndex);

                    Node* pNew = new Node(nodeValue);
                    pRoot = pNew;
                    *pCurr = pNew;

                    subCommand.erase(0, tmpIndex + 1);
                    commandSym = subCommand[0];
                    if(commandSym == DOWN_COMMAND)//get deeper
                    {
                        pNew->childs.push_back(nullptr);
                        BuildFrame(command, &(pNew->childs.back()), &pNew);
                        break;
                    }
                    else if(commandSym == UP_COMMAND)//go to the upper level
                    {
                        return nullptr;
                    }
                }
                case DOWN_COMMAND://go to child
                {
                    (*pCurr)->childs.push_back(nullptr);
                    BuildFrame(command, &(*pCurr)->childs.back(), pCurr);
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
    vector<Node*>::iterator it = pHead->childs.begin();
    while (it != pHead->childs.end())
    {
        bool bTmp = (it == (pHead->childs.end() - 1));
        PrettyPrint(*it, level, bTmp);
        it++;
    }
    (*level)--;
}

