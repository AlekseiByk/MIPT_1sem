#include "headers/FuncTree.h"
#include "headers/ASSert.h"
#include "headers/Text.h"
//#include "headers/Header.h"

unit_t arr[MAXCODELEN] = {};
int VarNumberArray[50] = {};
Namenumnum_t variables[VARNUMBER] = {};
int number_of_var = 0;
const char * s = "";
const char * starts = "";

int main() 
{
    int length = 0;

    int inputPhrase = 0;
    tree_node_t* head = nullptr;
    
    while (inputPhrase < 6)
    {
        printf("\n\n0.Из LangIn.txt в дерево\n1.Из StandartIn.txt(файл стандарта) в дерево\n2.в язык\n3.в асемблер\n4.графический дамп\n5.optimize\n6+.exit\n\n");
        scanf("%d", &inputPhrase);
        switch(inputPhrase)
        {
            case 0:
            {
                char* input = TextFromFile("files/LangIn.txt", &length);
                DeleteComments(input);
                printf("----/----\n%s\n----/----\n", input);

                input = ReCode(input, 1);
                printf("----/----\n%s\n----/----\n", input);

                number_of_var = 0;
                head = GetTree(input);
                free(input);

                MakeVarList(head);

                for (int i = 0; i < number_of_var; i++)
                    if (variables[ i ].num != -1)
                        printf("%d. %s -> %d -> %d\n", i, variables[ i ].name, variables[ i ].num, variables[ i ].numnum);
                preWriteTree("files/Standartout.txt", head);
                break;
            }
            case 1:
            {
                char *input = TextFromFile("files/StandartIn.txt", &length);
                number_of_var = 0;
                head = readTreeFrom(input);
                free(input);

                MakeVarList(head);

                for (int i = 0; i < number_of_var; i++)
                if (variables[ i ].num != -1)
                        printf("%d. %s -> %d -> %d\n", i, variables[ i ].name, variables[ i ].num, variables[ i ].numnum);
                preWriteTree("files/Standartout.txt", head);
                break;
            }
            case 2:
            {
                WriteTreeToLang("files/Langout.txt", head);
                break;
            }
            case 3:
            {
                FromTreeToAssembler("temp/input.asmc", head);
                system("make toasm");
                break;
            }
            case 4:
            {
                GraphTreeDump(head);
                break;
            }
            case 5:
            {
                head = treeOptimize(head);
                break;
            }
        }
    }
    /*char* input = TextFromFile(FILEIN, &length);

    head = treeOptimize(head);
    tree_node_t* 
    GraphTreeDump(head);
    preWriteTree("tree1.txt", head);
    

    input = TextFromFile("tree.txt", &length);
    tree_node_t* head = readTreeFrom(input);
    free(input);
    //GraphTreeDump(head2);
    GraphTreeDump(head);
    head = treeOptimize(head);
    GraphTreeDump(head);
    WriteTreeToLang("tree2.txt", head);

    
    FromTreeToAssembler("input.asmc", head);
    head = treeOptimize(head);

    GraphTreeDump(head);
*/
    treeDestruct(head);
    return 0;
}
