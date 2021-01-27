#include "FuncTree.h"
#include "functions.h"
#include "ASSert.h"


Namenum_t functionArray[MAXCOMMANDLEN] = {{"", 0}, {"+", 1}, {"-", 2}, {"*", 3}, {"/", 4}, {"^", 5}, {"cos", 6}, {"sin", 7}, {"abs", 8}, {"tg", 9}, {"ctg", 10}, {"sh", 11}, {"ch", 12}, {"th", 13}, {"cth", 14}, {"ln", 15}, {"log10", 16}, {"exp", 17}, {"acos", 18}, {"asin", 19}, {"atg", 20}, {"sqrt", 21}, {"get", 22}, {"put", 23}, {"deriv", 24}};
int FuncNumber = 25;

int CompChoose(const char * text)
{
	for (int i = 0; i < compnum; i++)
		if (strcmp(text, compare_operators[i].name) == 0)
			return i;
	return 0;
}

char* takefunc(int num)
{
	return functionArray[num].name;
}

int functionlist(const char *func)
{
	for(int i = 1; i < FuncNumber; i++)
		if (strcasecmp(functionArray[i].name, func) == 0)
			return i;
	strcpy (functionArray[FuncNumber].name, func);
	functionArray[FuncNumber].num = FuncNumber;
	FuncNumber++;
	return FuncNumber - 1;
}

tree_node_t* CreateNode(int type, double val, tree_node_t* node_left, tree_node_t* node_right)
{
	tree_node_t* node = (tree_node_t*) calloc(1, sizeof(tree_node_t));
    node -> data.type = type;
    node -> data.value = val;
	node -> left = node_left;
	if (node_left)
		node -> left -> parant = node;
	node -> right = node_right;
	if (node_right)
		node -> right -> parant = node;
	return node;
}

void Systemf(const char* format, ...)
{
    va_list arg;
    va_start(arg, format);

    char systemCom[MAXCOMMANDLEN];
    vsnprintf(systemCom, MAXCOMMANDLEN, format, arg);
    
    va_end(arg);
    system(systemCom);
}

void treeDestruct(tree_node_t* headNode)
{
    assert(headNode);
    
    if(headNode -> left !=  nullptr)
        treeDestruct(headNode -> left);

    headNode -> left = nullptr;

    if(headNode -> right !=  nullptr)
        treeDestruct(headNode -> right);

    headNode -> right = nullptr;

    if (headNode != nullptr)
        free(headNode);

    headNode = nullptr;
}

tree_node_t* TreeCopy(tree_node_t* node)
{
	assert(node);


	tree_node_t* temp = CreateNode(node -> data.type, node -> data.value);
	if (node -> left != nullptr)
	{
		temp -> left = TreeCopy(node -> left);
		temp -> left -> parant = temp;
	}
	if (node -> right != nullptr)
	{
		temp -> right = TreeCopy(node -> right);
		temp -> right -> parant = temp;
	}
	return temp;
}

int SearchForVar(tree_node_t* node,int variable)
{
	int count = 1;
	if (node -> left != nullptr)
		count *= SearchForVar(node -> left, variable);
	if (node -> right != nullptr)
		count *= SearchForVar(node -> right, variable);
	count *= !((node -> data.type == TYPEVARIABLE) && (node -> data.value == variable));
	return count;
}

#define PLUS(leftNode, rightNode)	CreateNode(TYPEFUNCTION, '+', leftNode, rightNode)
#define MINUS(leftNode, rightNode)	CreateNode(TYPEFUNCTION, '-', leftNode, rightNode)
#define MUL(leftNode, rightNode)	CreateNode(TYPEFUNCTION, '*', leftNode, rightNode)
#define DIV(leftNode, rightNode)	CreateNode(TYPEFUNCTION, '/', leftNode, rightNode)
#define FUNC(number, rightNode)		CreateNode(TYPEFUNCTION, number, 0, rightNode)
#define NNODE(number)				CreateNode(TYPENUMBER, number)

#define dL Differencial(node -> left, variable)
#define dR Differencial(node -> right, variable)
#define cL TreeCopy(node -> left)
#define cR TreeCopy(node -> right)
#define cN TreeCopy(node)		

tree_node_t* Differencial(tree_node_t* node, int variable)
{
	assert(node);

	switch(node -> data.type)
	{
		case TYPENUMBER:
			return CreateNode(TYPENUMBER, 0);
		case TYPEVARIABLE:
			if (node -> data.value == variable)
				return CreateNode(TYPENUMBER, 1);
			else
				return CreateNode(TYPENUMBER, 0);
			break;
		case TYPEFUNCTION:
			switch((int) node -> data.value)
			{
				case 1:
					return PLUS(dL, dR);
				case 2:
					return MINUS(dL, dR);
				case 3:
					return PLUS(MUL(dL, cR), MUL(dR, cL));
				case 4:
					return DIV(MINUS(MUL(dL, cR), MUL(dR, cL)), MUL(cR, cR));
				case 5:
					if (node -> right -> data.type == TYPENUMBER)
					{
						tree_node_t *temp = MUL(MUL(NNODE(node -> right -> data.value), cN), dL);
						(temp -> left -> right -> right -> data.value)--;
						return temp;
					}
					if (node -> left -> data.type == TYPENUMBER)
						return MUL(MUL(FUNC(15, NNODE(node -> left -> data.value)), cN), dR);
					printf("%d\n", SearchForVar(node -> right, variable));
					if (SearchForVar(node -> right, variable))
						return MUL(MUL(cR, CreateNode(TYPEFUNCTION, 5, cL, MINUS(cR, NNODE(1)))), dL);
					return MUL(cN, PLUS(MUL(cR, DIV(dL, cL)), MUL(dR, FUNC(15, cL))));
				case 6:
					return MUL(NNODE(-1), MUL(FUNC(1001, cR), dR));
				case 15:
					return MUL(DIV(NNODE(1), cR), dR);
				case 8:
					return MUL(DIV(cN, cR), dR);
				case 21:
					return DIV(MUL(NNODE(0.5), dR), cN);
				default:
					return MUL(FUNC(functions[(int) node -> data.value - 6].diff, cR), dR);
			}	
			break;
	}
	return nullptr;

}

#undef PLUS
#undef MINUS
#undef MUL
#undef DIV
#undef FUNC
#undef NNODE
#undef dL
#undef dR
#undef cL
#undef cR
#undef cN

int IfFunc(tree_node_t* node, int func)
{
	return (node -> data.type == TYPEFUNCTION) && (node -> data.value == func);
}

int IfNum(tree_node_t* node, double Num)
{
	return (node -> data.type == TYPENUMBER) && (node -> data.value == Num);
}

int IfNumber(tree_node_t* node)
{
	return (node -> data.type == TYPENUMBER);
}

tree_node_t* treeSolve(tree_node_t* node)
{
	assert(node);
	
	double a = 0, b = node -> right -> data.value;

	if (node -> left)
		a = node -> left -> data.value;

	if (IfFunc(node, '+'))
		b = a + b;
	if (IfFunc(node, '-'))
		b = a - b;
	if (IfFunc(node, '*'))
		b = a * b;
	if (IfFunc(node, '/'))
		b = a / b;
	if (IfFunc(node, '^'))
		b = exp(log(a) * b);

	if ((node -> data.type == TYPEFUNCTION) && (node -> data.value >= 6))
		b = functions[(int) node -> data.value - 6].func(b);

	treeDestruct(node);
	node = CreateNode(TYPENUMBER, b);
	return node;
}

#define OPTFUNCNUMBER(pos1, pos2, func, num)								\
		if (IfFunc(node, func) && IfNum(node -> pos1, num))					\
		{																	\
			tree_node_t* temp = node -> pos2;								\
			node -> pos2 = nullptr;											\
			treeDestruct(node);												\
			(*counter)++;													\
			return temp;													\
		}
#define MULMULOPT(node1, node2, node3, op1, op2)													\
		if (IfFunc(node, op1) && IfFunc(node1, op2) && IfNumber(node3) && IfNumber(node2) )			\
		{																							\
			double a = node2 -> data.value, b = node3 -> data.value;								\
			node3 -> data.value = a * b;															\
			node2 -> data.value = 1;																\
			(*counter)++;																			\
		}
#define SUMSUMOPT(node1, node2, node3, op1, op2)													\
		if (IfFunc(node, op1) && IfFunc(node1, op2) && IfNumber(node3) && IfNumber(node2) )			\
		{																							\
			double a = node2 -> data.value, b = node3 -> data.value;								\
			node3 -> data.value = a + b;															\
			node2 -> data.value = 0;																\
			(*counter)++;																			\
			return node;																			\
		}
#define DIVDIVOPT(node1, node2, node3, op1, op2)													\
		if (IfFunc(node, op1) && IfFunc(node1, op2) && IfNumber(node3) && IfNumber(node2) )			\
		{																							\
			double a = node2 -> data.value, b = node3 -> data.value;								\
			node3 -> data.value = a / b;															\
			node2 -> data.value = 1;																\
			(*counter)++;																			\
			return node;																			\
		}
#define SUBSUBOPT(node1, node2, node3, op1, op2)													\
		if (IfFunc(node, op1) && IfFunc(node1, op2) && IfNumber(node3) && IfNumber(node2) )			\
		{																							\
			double a = node2 -> data.value, b = node3 -> data.value;								\
			node3 -> data.value = a - b;															\
			node2 -> data.value = 0;																\
			(*counter)++;																			\
			return node;																			\
		}

tree_node_t* treeNodeOptimize(tree_node_t* node, int* counter)
{
	assert(node);

	if ((node -> left != nullptr) && (node -> left -> data.type != TYPENUMBER))
		node -> left = treeNodeOptimize(node -> left, counter);
	if ((node -> right != nullptr) && (node -> right -> data.type != TYPENUMBER))
		node -> right= treeNodeOptimize(node -> right, counter);

	if (IfFunc(node, '*') && (IfNum(node -> left, 0) || IfNum(node -> right, 0)))
	{
		treeDestruct(node);
		node = CreateNode(TYPENUMBER, 0);
		(*counter)++;
		return node;
	}
	OPTFUNCNUMBER(left, right, '*', 1);
	OPTFUNCNUMBER(right, left, '*', 1);
	OPTFUNCNUMBER(right, left, '/', 1);
	OPTFUNCNUMBER(left, right, '+', 0);
	OPTFUNCNUMBER(right, left, '+', 0);
	OPTFUNCNUMBER(right, left, '-', 0);
	OPTFUNCNUMBER(right, left, '^', 1);
	if ((node -> data.type == TYPEFUNCTION) && (!(node -> left) || (node -> left -> data.type == TYPENUMBER)) && (node -> right -> data.type == TYPENUMBER))
	{
		(*counter)++;
		return treeSolve(node);
	}
	MULMULOPT(node -> left, node -> right, node -> left -> left, '*', '*');
	MULMULOPT(node -> left, node -> right, node -> left -> right, '*', '*');//
	MULMULOPT(node -> right, node -> left, node -> right -> left, '*', '*');//
	MULMULOPT(node -> right, node -> left, node -> right -> right, '*', '*');//
	MULMULOPT(node -> left, node -> right, node -> left -> left, '*', '/');//
	MULMULOPT(node -> right, node -> left, node -> right -> left, '*', '/');//
	MULMULOPT(node -> right, node -> right -> right, node -> left, '/', '/');
	SUMSUMOPT(node -> left, node -> right, node -> left -> left, '+', '+');
	SUMSUMOPT(node -> left, node -> right, node -> left -> right, '+', '+');
	SUMSUMOPT(node -> right, node -> left, node -> right -> left, '+', '+');
	SUMSUMOPT(node -> right, node -> left, node -> right -> right, '+', '+');
	SUMSUMOPT(node -> right, node -> right -> right, node -> left, '-', '-');
	SUMSUMOPT(node -> left, node -> right, node -> left -> right, '-', '-');
	DIVDIVOPT(node -> left, node -> right, node -> left -> right, '/', '/');
	DIVDIVOPT(node -> right, node -> left, node -> right -> right, '/', '/');
	if (IfFunc(node, '*') && IfFunc(node -> left, '/') && IfNumber(node -> right) && IfNumber(node->left->right) )
		{
			double a = node -> right -> data.value, b = node->left->right -> data.value;
			node->left->right -> data.value = b / a;
			node -> right -> data.value = 1;
			(*counter)++;
			return node;
		}
	if (IfFunc(node, '/') && IfFunc(node -> left, '*') && IfNumber(node -> right) && IfNumber(node->left->right) )
		{
			double a = node -> right -> data.value, b = node->left->right -> data.value;
			node->right -> data.value = a / b;
			node -> left -> right -> data.value = 1;
			(*counter)++;
			return node;
		}
	SUBSUBOPT(node -> left, node -> left -> right, node -> right, '-', '+');
	return node;
}

#undef OPTFUNCNUMBER
#undef SUBSUBOPT
#undef SUMSUMOPT
#undef DIVDIVOPT
#undef MULMULOPT

tree_node_t* treeOptimize(tree_node_t* node)
{
	int k = 1;
    while (k)
    {
    	k = 0;
    	node = treeNodeOptimize(node, &k);
    }
    return node;
}
