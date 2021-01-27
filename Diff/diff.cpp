#include "FuncTree.h"
#include "ASSert.h"
#include "Text.h"

char TREEFILE[] = "equation.txt";

int main() 
{
    int length = 0;
    char* input = TextFromFile(TREEFILE, &length);
    tree_node_t* head = readTreeFrom(input, length);
    free(input);
    LatexWriteBegin("Latex");
    intWriteTree(TREEFILE, head);
    GraphTreeDump(head);

    tree_node_t* head1 = Differencial(head);
    tree_node_t* head2 = Differencial(head1);

    head1 = treeOptimize(head1);
    GraphTreeDump(head1);
    head2 = treeOptimize(head2);
    GraphTreeDump(head2);

    LatexWrite(head, "f  ","Latex");
    LatexWrite(head1, "f\'  ","Latex");
    LatexWrite(head2, "f\'\'  ","Latex");

    treeDestruct(head2);
    treeDestruct(head1);
    treeDestruct(head);

	LatexWriteEnd("Latex");

   
}
