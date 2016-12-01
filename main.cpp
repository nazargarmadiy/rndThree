#include <threetest.h>

int main()
{
    /*RndTree tree;
    tree.PrintTree();
    tree.Add("node_2");
    tree.Add("node_1");
    tree.PrintTree();
    Node* node = tree.FindNode("node_2");
    printf("\n-------\n");
    tree.Add("node_2_1", node);
    Node* node2 = tree.Add("node_2_2", node);
    node = tree.Add("node_2_3", node);
    tree.PrintTree();
    printf("\n-------\n");
    Node* node3 = tree.Add("node_3_1", node2);
    tree.Add("node_3_2", node3);
    tree.Add("node_3_3", node2);
    tree.PrettyPrint();
    string comm = tree.GetCommand();
    printf("\n%s\n", comm.c_str());
    tree.BuildTree(comm);

    printf("\n-------\n");
    tree.PrettyPrint();*/

    ThreeTest test;
    int count = 20;
    RndTree *pThree = test.CreateRndThree(&count);
    pThree->PrettyPrint();

    string comm = pThree->Serialize();
    RndTree unboxed;
    unboxed.Deserialize(comm);
    unboxed.PrettyPrint();

    string unboxedCommand = unboxed.Serialize();
    printf("\nTest: serialized objects are %s.\n", comm.compare(unboxedCommand) ? "different" : "same");
    //printf("\n%s", comm.c_str());
    return 0;
}
