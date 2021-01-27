#include "FuncTree.h"
#include "ASSert.h"
#include "Text.h"
#include <ctype.h>

char FILEIN[] = "equation.txt";
const int MAXCODELEN = 10000;
const char * s = "";
const char * starts = "";
const int VARNUMBER = 100;
unit_t arr[MAXCODELEN] = {};
Namenum_t variables[VARNUMBER] = {};

int number_of_var = 0;

tree_node_t* GetG();
tree_node_t* GetExpression();
tree_node_t* GetNumber();
tree_node_t* GetT();
tree_node_t* GetK();
tree_node_t* GetE();
tree_node_t* GetP();
tree_node_t* GetId();

tree_node_t* GetTree(const char* str);
tree_node_t* GetGeneral();
tree_node_t* GetMainCode();
tree_node_t* GetAsign();
tree_node_t* GetFunc();
tree_node_t* GetOp(int count);
tree_node_t* GetObject(int count);
tree_node_t* GetIf(int count);
tree_node_t* GetWhile(int count);
tree_node_t* GetArg();
tree_node_t* GetComp();

int findVariable(const char * val, int VarNum = 1);
void printERROR(const char *text);
void SkipSpaces();
tree_node_t* readTreeFrom(char* input);
void DeleteComments(char *code);

void GraphTreeDump(tree_node_t* headNode);
void preWriteTree(const char *FileOut, tree_node_t* headNode);
char* ReCode(char* input, int num);
void WriteTreeToLang(const char *FileOut, tree_node_t* headNode);
void FromTreeToAssembler(const char *FileOut, tree_node_t* headNode);
void MakeVarList(tree_node_t* node);

int main() 
{
    int length = 0;
    char* input = TextFromFile(FILEIN, &length);

    DeleteComments(input);
    printf("----/----\n%s\n----/----\n", input);

    input = ReCode(input, 1);
    printf("----/----\n%s\n----/----\n", input);

    tree_node_t* head = GetTree(input);
    free(input);
    GraphTreeDump(head);
    preWriteTree("tree1.txt", head);

    //input = TextFromFile("tree.txt", &length);
    //tree_node_t* head2 = readTreeFrom(input);
    //free(input);
    //GraphTreeDump(head2);
    WriteTreeToLang("tree2.txt", head);

    MakeVarList(head);

    for (int i = 0; i < number_of_var; i++)
        printf("%d. %s -> %d\n", i + 1, variables[ i ].name, variables[ i ].num);

    head = treeOptimize(head);

    GraphTreeDump(head);

    treeDestruct(head);
    //treeDestruct(head2);
    return 0;
}

int findVariable(const char * var, int VarNum)
{
    for (int i = 0; i < number_of_var; i++)
    {
        if ((strcmp(var, variables[i].name) == 0) && (variables[i].num == VarNum) )
            return i;
    }

    strcpy(variables[number_of_var].name, var);
    variables[number_of_var].num = VarNum;

    number_of_var++;

    return number_of_var - 1;
}

#include "Language.h"
#include "Print.h"

void _toassembler(FILE* file, tree_node_t* node)
{

}


void FromTreeToAssembler(const char *FileOut, tree_node_t* node)
{
    assert(node);

    FILE* Write = fopen(FileOut, "w");
    assert(Write);

    _toassembler(Write, node);
    
    fclose(Write);
}



void MakeVarList(tree_node_t* node)
{
    static int VarVar = 0;

    if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == SEMICOLONop) && ((node -> left -> data.type) == TYPEOPERATOR) && ((node -> left -> data.value) == ASSIGNop))
        if ((node -> left -> left -> data.type) == TYPEVARIABLE)
            variables[(int) node -> left -> left -> data.value].num = 0;

    if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == SEMICOLONop) && ((node -> left -> data.type) == TYPEFUNCTION))
        VarVar++;

    if ((node -> data.type) == TYPEVARIABLE)
        if ((variables[(int) node -> data.value].num != VarVar) && (variables[(int) node -> data.value].num != 0))
            findVariable(variables[(int) node -> data.value].name, VarVar);

    if ((node -> left) != nullptr)
        MakeVarList(node -> left);
    if ((node -> right) != nullptr)
        MakeVarList(node -> right);
}