#include "FuncTree.h"
#include "functions.h"
#include "ASSert.h"

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

tree_node_t* readTreeFrom(char* input,int len)
{
    assert(input);
    assert(len > 0);
    int k = 0;
    tree_node_t* temp = CreateNode();
    tree_node_t* head = temp;
    int i = 1;
    while (i < len)
    {
	    if ((input[i] == '(') && (k == 0))
	    {
	    	tree_node_t* temp1 = CreateNode();
	    	temp -> left = temp1;
	    	temp1 -> parant = temp;
	    	temp = temp1;
	    	i++;
	    }
	    else if ((input[i] == '(') && (k == 1))
	    {
	    	tree_node_t* temp1 = CreateNode();
	    	temp -> right = temp1;
	    	temp1 -> parant = temp;
	    	temp = temp1;
	    	k = 0;
	    	i++;
	    }
	    else if (input[i] == ')')
	    {
	    	temp = temp -> parant;
	    	k = 1;
	    	i++;
	    }
	    else
	    {
	    	char func[FUNCTIONMAXLEN] = {};
	    	int n = 0;
	    	double number = 0;
	    	int scan = sscanf(input + i, "%lf%n", &number, &n);
	    	if (scan == 1)
	    	{
	    		temp -> data.type = TYPENUMBER;
	    		temp -> data.value = number;
	    		i += n;
	    	}
	    	else
	    	{
	    		scan = sscanf(input + i, "%[a-zA-Z]%n", func, &n);
	    		if ((n == 1) && (scan == 1))
	    		{
	    			temp -> data.type = TYPEVARIABLE;
	    			temp -> data.value = (char) func[0];
	    			i += n;	    			
	    		}
	    		else
	    		{
	    			sscanf(input + i, "%[^()]%n", func, &n);
	    			temp -> data.type = TYPEFUNCTION;
	    			k = 1;
	    			if (n == 1)
	    				temp -> data.value = (char) func[0];
	    			else
	    				temp -> data.value = FindFunctionByName(func); //////////////////////////////////////////////////////////////func number
	    			i += n;
	    		}
	    	}
	    }
	}
    return head;  
}

void GraphPrintTree(FILE *dump, tree_node_t* headNode)
{
	assert(dump);

	if (headNode -> data.type == TYPENUMBER)
    	fprintf(dump, "\"%p\"[label = \"{<data> %.4lf}\", color = \"red\"];\n", headNode, headNode -> data.value);
    else if (headNode -> data.type == TYPEVARIABLE)
    	fprintf(dump, "\"%p\"[label = \"{<data> %c}\", color = \"green\"];\n", headNode, (int) headNode -> data.value);
    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value < 256))
    	fprintf(dump, "\"%p\"[label = \"{<data> %c}\", color = \"blue\"];\n", headNode, (int) headNode -> data.value);
    else
    	fprintf(dump, "\"%p\"[label = \"{<data> %s}\", color = \"blue\"];\n", headNode, FindFunctionByNumber( (int) headNode -> data.value));                                                        

    /*if (headNode -> parant)
    	fprintf(dump, "\"%p\":data -> \"%p\":data[color = \"red\"\n];", headNode, headNode -> parant);*/

    if(headNode -> left !=  nullptr)
    {
        fprintf(dump, "\"%p\":data -> \"%p\":data;\n", headNode, headNode -> left);
        GraphPrintTree(dump, headNode -> left);
    }
    if(headNode -> right !=  nullptr)
    {
        fprintf(dump, "\"%p\":data -> \"%p\":data;\n", headNode, headNode -> right);
        GraphPrintTree(dump, headNode -> right);
    }
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

void GraphTreeDump(tree_node_t* headNode)
{
    assert(headNode);

    static unsigned int DumpNumber = 0;

    char FileIn[11] = "";
    snprintf(FileIn, sizeof(FileIn), "Dump%02d.dot", DumpNumber);
    FILE* dump = fopen(FileIn, "wb");
    assert(dump);

    fprintf(dump, "digraph listDump\n{\nnode [shape = record];\n");

    GraphPrintTree(dump, headNode);
    
    fprintf(dump, "}");
    fclose(dump);

    Systemf("dot -Tpdf  Dump%02d.dot -o Dump%02d.pdf", DumpNumber, DumpNumber);
    Systemf("rm Dump%02d.dot", DumpNumber);
    //Systemf("evince Dump%02d.pdf", DumpNumber);

    DumpNumber++;    
}

void WriteTree(FILE *Write, tree_node_t* headNode)
{
	assert(headNode);
	assert(Write);

    fprintf(Write, "(");

    if(headNode -> left !=  nullptr)
        WriteTree(Write, headNode -> left);
    if (headNode -> data.type == TYPENUMBER)
        fprintf(Write, "%.4lf", headNode -> data.value);
    else if (headNode -> data.type == TYPEVARIABLE)
        fprintf(Write, "%c", (int) headNode -> data.value);
    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value < 256))
        fprintf(Write, "%c", (int) headNode -> data.value);
    else
    	fprintf(Write, "%s", FindFunctionByNumber((int) headNode -> data.value));

    if(headNode -> right !=  nullptr)
        WriteTree(Write, headNode -> right);

    fprintf(Write, ")");
}

void intWriteTree(char FileOut[], tree_node_t* headNode)
{
    assert(headNode);
    FILE* Write = fopen(FileOut, "w");
    assert(Write);

    WriteTree(Write, headNode);
    
    fclose(Write);
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

int FindFunctionByName(char func[])
{
	for(int i = 0; i < FuncNumber; i++)
		if (strcasecmp(functions[i].name, func) == 0)
			return functions[i].value;
	return 0;
}

char* FindFunctionByNumber(int func)
{
	if (func >= 1000 && func <= 1000 + FuncNumber)
		return functions[func - 1000].name;
	return 0;
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
				case '+':
					return PLUS(dL, dR);
				case '-':
					return MINUS(dL, dR);
				case '*':
					return PLUS(MUL(dL, cR), MUL(dR, cL));
				case '/':
					return DIV(MINUS(MUL(dL, cR), MUL(dR, cL)), MUL(cR, cR));
				case '^':
					if (node -> right -> data.type == TYPENUMBER)
					{
						tree_node_t *temp = MUL(MUL(NNODE(node -> right -> data.value), cN), dL);
						(temp -> left -> right -> right -> data.value)--;
						return temp;
					}
					if (node -> left -> data.type == TYPENUMBER)
						return MUL(MUL(FUNC(1009, NNODE(node -> left -> data.value)), cN), dR);
					printf("%d\n", SearchForVar(node -> right, variable));
					if (SearchForVar(node -> right, variable))
						return MUL(MUL(cR, CreateNode(TYPEFUNCTION, '^', cL, MINUS(cR, NNODE(1)))), dL);
					return MUL(cN, PLUS(MUL(cR, DIV(dL, cL)), MUL(dR, FUNC(1009, cL))));
				case 1000:
					return MUL(NNODE(-1), MUL(FUNC(1001, cR), dR));
				case 1009:
					return MUL(DIV(NNODE(1), cR), dR);
				case 1002:
					return MUL(DIV(cN, cR), dR);
				case 1015:
					return DIV(MUL(NNODE(0.5), dR), cN);
				default:
					return MUL(FUNC(functions[(int) node -> data.value - 1000].diff, cR), dR);
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

	if ((node -> data.type == TYPEFUNCTION) && (node -> data.value >= 1000))
		b = functions[(int) node -> data.value - 1000].func(b);

	treeDestruct(node);
	node = CreateNode(TYPENUMBER, b);
	return node;
}

#define OPTFUNCNUMBER(pos1, pos2, func, num)								\
		if (IfFunc(node, func) && IfNum(node -> pos1, num))					\
		{																	\
			tree_node_t* temp = TreeCopy(node -> pos2);						\
			treeDestruct(node);												\
			(*counter)++;													\
			return temp;													\
		}
#define MULMULOPT(node1, node2, node3)										\
		if (IfFunc(node1, '*') && IfNumber(node3))							\
		{																	\
			double a = node2 -> data.value, b = node3 -> data.value;		\
			node2 -> data.value = a * b;									\
			node3 -> data.value = 1;										\
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
	/*if (IfFunc(node, '*') && ((IfNumber(node -> left) && IfFunc(node -> right, '*') && (IfNumber(node -> right -> left) || IfNumber(node -> right -> right))) ||
		   					  (IfNumber(node -> right) && IfFunc(node -> left, '*') && (IfNumber(node -> left -> left) || IfNumber(node -> left -> right)))))
	{
		MULMULOPT(node -> left, node -> right, node -> left -> left);
		MULMULOPT(node -> left, node -> right, node -> left -> right);
		MULMULOPT(node -> right, node -> left, node -> right -> left);
		MULMULOPT(node -> right, node -> left, node -> right -> right);
		(*counter)++;
	}*/

	return node;
}

#undef OPTFUNCNUMBER

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

void printSkobka(FILE *f, tree_node_t* headNode)
{
	fprintf(f, "(");
	LatexWriteNode(f, headNode);
	fprintf(f, ")");
}

void LatexWriteNode(FILE* dump, tree_node_t* headNode)
{

	assert(headNode);
	assert(dump);

    //fprintf(dump, "(");
    if(headNode -> left !=  nullptr)
    {
    	if (IfFunc(headNode, '*') && (IfFunc(headNode -> left, '+') || IfFunc(headNode -> left, '-')))
    		printSkobka(dump, headNode -> left);
    	else if (IfFunc(headNode, '^') && (headNode -> left -> data.type == TYPEFUNCTION))
    		printSkobka(dump, headNode -> left);
    	else if (IfFunc(headNode, '/'))
    	{
    		fprintf(dump, "\\dfrac{");
    		LatexWriteNode(dump, headNode-> left);
    		fprintf(dump, "}");
    	}
    	else 
    		LatexWriteNode(dump, headNode-> left);
    }
 



    if (headNode -> data.type == TYPENUMBER)
    	if (headNode -> data.value == (int) headNode -> data.value)
        	fprintf(dump, "%d", (int) headNode -> data.value);
        else
        	fprintf(dump, "%.4lf", headNode -> data.value);
    else if (headNode -> data.type == TYPEVARIABLE)
        fprintf(dump, "%c", (int) headNode -> data.value);

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value >= 1000))
        fprintf(dump, "%s", FindFunctionByNumber((int) headNode -> data.value));
    else if (IfFunc(headNode, '*'))
		fprintf(dump, "\\cdot ");
	 else if (IfFunc(headNode, '^') || IfFunc(headNode, '+') || IfFunc(headNode, '-'))
		fprintf(dump, "%c", (int) headNode -> data.value);


    if(headNode -> right !=  nullptr)
    {
		if (IfFunc(headNode, '*') && (IfFunc(headNode -> right, '+') || IfFunc(headNode -> right, '-')))
    		printSkobka(dump, headNode -> right);
    	else if (IfFunc(headNode, '^') && (headNode -> right -> data.type == TYPEFUNCTION))
		{
			fprintf(dump, "{");
			LatexWriteNode(dump, headNode -> right);
			fprintf(dump, "}");
		}
    	else if (IfFunc(headNode, '/'))
    	{
    		fprintf(dump, "{");
    		LatexWriteNode(dump, headNode-> right);
    		fprintf(dump, "}");
    	}
    	else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value >= 1000))
	    	printSkobka(dump, headNode -> right);
	    else 
	    	LatexWriteNode(dump, headNode-> right);
    }

    //fprintf(dump, ")");
}

void LatexWriteBegin(const char* file)
{
	char FileIn[50] = "";
    snprintf(FileIn, sizeof(FileIn), "%s.tex", file);
    FILE* dump = fopen(FileIn, "wb");
    assert(dump);

    fprintf(dump, "\\documentclass{article}\n\\usepackage[utf8]{inputenc}\n\\usepackage[russian]{babel}\n\\usepackage{setspace,amsmath}\n\\begin{document}\n\n\n");
    
    fclose(dump);
} 

void LatexWriteEnd(const char* file)
{
	char FileIn[50] = "";
    snprintf(FileIn, sizeof(FileIn), "%s.tex", file);
    FILE* dump = fopen(FileIn, "a");
    assert(dump);
    
    fprintf(dump, "\n\n\n\\end{document}\n");
    fclose(dump);

    Systemf("pdflatex %s.tex", file);
    Systemf("rm %s.log", file);
    Systemf("rm %s.aux", file);  
}
void LatexWrite(tree_node_t* headNode, const char* text, const char* file)
{
	char FileIn[50] = "";
    snprintf(FileIn, sizeof(FileIn), "%s.tex", file);
    FILE* dump = fopen(FileIn, "a");
    assert(dump);

	fprintf(dump, "%s = $", text);
	LatexWriteNode(dump, headNode);
	fprintf(dump, "$\n\n\n\n");
	fclose(dump);
}