#include "headers/FuncTree.h"
#include "headers/ASSert.h"
#include "headers/Text.h"
#include <ctype.h>

char FILEIN[] = "equation.txt";

const char * s = "";
const char * starts = "";

tree_node_t* GetG(const char* str);
tree_node_t* GetN();
tree_node_t* GetT();
tree_node_t* GetE();
tree_node_t* GetP();
tree_node_t* GetId();
tree_node_t* GetK();
void printERROR(const char *text);

int main() 
{
    int length = 0;
    char* input = TextFromFile(FILEIN, &length);
    tree_node_t* head = GetG(input);

    printf("1ok\n");
    fflush(0);

    free(input);
    LatexWriteBegin("Latex");
    LatexWriteText("Latex", "у нас есть");
    LatexWrite("Latex", head, "f  ");
    GraphTreeDump(head);
    head = treeOptimize(head);

    printf("2ok\n");
    fflush(0);

    GraphTreeDump(head);
    LatexWriteText("Latex", "Получаем");
    LatexWrite("Latex", head, "f  ");

    tree_node_t* head1 = Differencial(head);
    LatexWriteText("Latex", "Простейше взяв производную");
    LatexWrite("Latex", head1, "f\'  ");
    GraphTreeDump(head1);

    head1 = treeOptimize(head1);
    LatexWriteText("Latex", "в конце преобразований получим");
    LatexWrite("Latex", head1, "f\'  ");
    GraphTreeDump(head1);

    treeDestruct(head);
    treeDestruct(head1);

    LatexWriteEnd("Latex");
    return 0;
}

tree_node_t* GetG(const char* str)
{
    s = str;
    starts = s;
    tree_node_t* val = GetE();
    if (*s != '\0')
    {
        printERROR("the last symbol is no a \'\\0\'");
    }
    return val;
}

tree_node_t* GetN()
{
    double val = 0;
    int n = 0;
    sscanf( s,"%lf%n", &val, &n);
    s += n;
    tree_node_t*node= CreateNode(TYPENUMBER, val);
    return node;
}

tree_node_t* GetT()
{
    tree_node_t* node = GetK();
    while (*s == '*' || *s == '/')
    {
        char OP = *s;
        s++;
        tree_node_t* node2 = GetK();
        node = CreateNode(TYPEFUNCTION, OP, node, node2);
    }
    return node;
}

tree_node_t* GetK()
{
    tree_node_t* node = GetP();
    if (*s == '^')
    {
        s++;
        tree_node_t* node2 = GetP();
        node = CreateNode(TYPEFUNCTION, '^', node, node2);
    }
    return node;
}

tree_node_t* GetE()
{
    tree_node_t* node = nullptr;
    node = GetT();
    while (*s == '+' || *s == '-')
    {
        char OP = *s;
        s++;
        tree_node_t* node2 = GetT();
        node = CreateNode(TYPEFUNCTION, OP, node, node2);
    }
    return node;
}

tree_node_t* GetP()
{
    if (*s == '(')
    {
        s++;
        tree_node_t* node = GetE();
        if (*s != ')')
        {
            printERROR("missing \')\'");
        }
        s++;
        return node;
    }
    if (isalpha(*s))
        return GetId();

    return GetN();
}

tree_node_t* GetId()
{
    char func[FUNCTIONMAXLEN] = {};
    int n = 0;
    sscanf(s, "%[A-Za-z]%n", func, &n);
    s+=n;
    tree_node_t* node = nullptr;
    if(*s == '(')
        node = CreateNode(TYPEFUNCTION, FindFunctionByName(func), nullptr, GetP());
    else
        node = CreateNode(TYPEVARIABLE, func[0]);
    return node;
}

void printERROR(const char *text)
{
    if (s - starts >= 8)
        printf("Syntax Error: %s\n\t%-12s\n\t\t^\n\t\t%lu\n", text, s - 8, s - starts);
    else
        printf("Syntax Error: %s\n\t%-12s\n\t   ^^^^^^\n\t     %lu\n", text, starts, s - starts);
    fflush(0);
    abort();
}