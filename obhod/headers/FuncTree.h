#ifndef TREEFUNC_H
#define TREEFUNC_H

#include <stdio.h>
//#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>


const int MAXCOMMANDLEN = 200;
const int FUNCTIONMAXLEN = 20;

///------------------------------------------------------------------------------------------------------------
///
///------------------------------------------------------------------------------------------------------------
struct node_value_t{
	int type;
	double value;
};

enum types{
	TYPENUMBER = 1,
	TYPEFUNCTION = 2,
	TYPEVARIABLE = 3
};

struct tree_node_t
{
    node_value_t data;
    tree_node_t* left;
    tree_node_t* right;
    tree_node_t* parant;
};
int IfFunc(tree_node_t* node, int func);
int IfNum(tree_node_t* node, double Num);
int IfNumber(tree_node_t* node);
void GraphTreeDump(tree_node_t* headNode);
tree_node_t* readTreeFrom(char* input,int length);
void intWriteTree(char FileOut[], tree_node_t* headNode);
tree_node_t* TreeCopy(tree_node_t* node);
void treeDestruct(tree_node_t* headNode);
int FindFunctionByName(char func[]);
char* FindFunctionByNumber(int func);
tree_node_t* Differencial(tree_node_t* headNode, int variable = 'x');
tree_node_t* treeOptimize(tree_node_t* node);
void LatexWrite(tree_node_t* head);
void LatexWriteNode(FILE* dump, tree_node_t* headNode);
void LatexWrite(const char* file, tree_node_t* headNode, const char* text);
void LatexWriteEnd(const char* file);
void LatexWriteBegin(const char* file);
tree_node_t* CreateNode(int type = 0, double val = 0, tree_node_t* node_left = 0, tree_node_t* node_right = 0);
void LatexWriteText(const char* file, const char* text);
#endif
