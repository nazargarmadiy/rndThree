#include "rndtree.h"

Node::Node(string _val)
{
    value = _val;
    childs = vector<Node*>();
}

RndTree::RndTree()
{
    pRoot == nullptr;
}

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

void RndTree::PrintTree(Node *pHead)
{
    if(!pHead && !pRoot)
    {
        printf("Tree is empty.\n");
        return;
    }
    if(!pHead)
    {
        PrintTree(pRoot);
        return;
    }
    printf("%s (has %d childs)\n", pHead->value.c_str(), pHead->childs.size());
    vector<Node*>::iterator it = pHead->childs.begin();
    while (it != pHead->childs.end() )
    {
        PrintTree(*it);
        it++;
    }
}

void RndTree::GetCommand(Node *pHead, Node **pParrent, string *pCommand)
{
    if(!pHead)
        return;
    pCommand->append("V:");
    pCommand->append(pHead->value);

    pCommand->append(" C:");
    pCommand->append(to_string(pHead->childs.size()));
    pCommand->append(";");

    for(int i = 0; i < pHead->childs.size(); i++)
    {
        pCommand->append("D:");
        pCommand->append(to_string(i));
        pCommand->append("\n");
        GetCommand(pHead->childs[i], nullptr, pCommand);
        pCommand->append("U\n");
    }
}

string RndTree::GetCommand()
{
    string* command = new string();
    if(!pRoot)
        return *command;
    GetCommand(pRoot, &pRoot, command);
    command->append("end;");
    return *command;
}

Node* RndTree::BuildFrame(string *command, Node **pCurr, Node **pParr)
{
    Node* pRoot;

    while(command->compare("end;"))
    {
        int index = command->find_first_of('\n');
        string subCommand = command->substr(0, index);
        command->erase(0, index + 1);

        /*if(!subCommand.compare("end;"))
            return nullptr;*/

        char commandSym = subCommand[0];
        switch (commandSym)
        {
            case 'V'://vrite new node
            {
                subCommand.erase(0, 2);
                int tmpIndex = subCommand.find("C:");
                string nodeValue =subCommand.substr(0, tmpIndex -1);

                Node* pNew = new Node(nodeValue);
                pRoot = pNew;
                *pCurr = pNew;

                subCommand.erase(0, tmpIndex +2);
                tmpIndex = subCommand.find(';');
                string cntString = subCommand.substr(0, tmpIndex);
                int childs = atoi(cntString.c_str());

                for(int i = 0; i < childs; i++)
                {
                    pNew->childs.push_back(nullptr);
                }
                subCommand.erase(0, tmpIndex +1);
                commandSym = subCommand[0];
                if(commandSym == 'D')//get deeper
                {
                    subCommand.erase(0, 2);
                    int index = atoi(subCommand.c_str());
                    BuildFrame(command, &(pNew->childs[index]), &pNew);
                    break;
                }
                else if(commandSym == 'U')//go to the upper level
                {
                    return nullptr;
                }
            }
            case 'D'://go to child
            {
                subCommand.erase(0, 2);
                int index = atoi(subCommand.c_str());
                BuildFrame(command, &(*pCurr)->childs[index], pCurr);
                break;
            }
            case 'U'://return to upper level
            {
                return nullptr;
            }
            default://unknown kommand
                break;
        }
    }
    return pRoot;
}

Node* RndTree::BuildTree(string command)
{
    Node *pHead;
    pRoot = BuildFrame(&command, &pHead, &pHead);
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

