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

struct unit_t{
    int type;
    int value;
};

struct Namenum_t{
    char name[FUNCTIONMAXLEN];
    int num = 0;
};

struct tree_node_t
{
    node_value_t data;
    tree_node_t* left;
    tree_node_t* right;
    tree_node_t* parant;
};

struct system_struct{
	char name[MAXCOMMANDLEN];
	char color[MAXCOMMANDLEN];
	char form[MAXCOMMANDLEN];
};

enum types{
	TYPENUMBER 		= 1,
	TYPEFUNCTION 	= 2,
	TYPEVARIABLE 	= 3,
	TYPEOPERATOR	= 4,
	TYPECOMPARE		= 5,

	OPop 		= 0,
	SEMICOLONop	= 1,
	COMMAop		= 2,
	WHILEop 	= 3,
	IFop		= 4,
	IFELSEop	= 5,
	RETURNop 	= 6,
	ASSIGNop	= 7,
};

const system_struct operators[] = { { "op", 		"orange", "circle"}, 
									{ ";", 			"orange", "circle"},
									{ ",", 			"orange", "circle"},
									{ "while", 		"orange", "pentagon"},
									{ "if", 		"orange", "hexagon"},
									{ "if-else", 	"orange", "hexagon"},
									{ "ret", 	"orange", "septagon"},
									{ "=", 			"orange", "larrow"}
								};

const Namenum_t compare_operators[] = {	{ "==", 0}, { "!=", 0}, { ">", 0}, { ">=", 0}, { "<", 0}, { "<=", 0} };
const int compnum = 6;

void Systemf(const char* format, ...);
int IfFunc(tree_node_t* node, int func);
int IfNum(tree_node_t* node, double Num);
int IfNumber(tree_node_t* node);
void treeDestruct(tree_node_t* headNode);
tree_node_t* TreeCopy(tree_node_t* node);
tree_node_t* Differencial(tree_node_t* headNode, int variable = 'x');
tree_node_t* treeOptimize(tree_node_t* node);
tree_node_t* CreateNode(int type = 0, double val = 0, tree_node_t* node_left = 0, tree_node_t* node_right = 0);
int CompChoose(const char * text);
int functionlist(const char *func);
char* takefunc(int num);
#endif
