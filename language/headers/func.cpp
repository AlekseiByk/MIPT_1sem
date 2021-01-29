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

#define PLUS(leftNode, rightNode)	CreateNode(TYPEFUNCTION, 1, leftNode, rightNode)
#define MINUS(leftNode, rightNode)	CreateNode(TYPEFUNCTION, 2, leftNode, rightNode)
#define MUL(leftNode, rightNode)	CreateNode(TYPEFUNCTION, 3, leftNode, rightNode)
#define DIV(leftNode, rightNode)	CreateNode(TYPEFUNCTION, 4, leftNode, rightNode)
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

	if (IfFunc(node, 1))
		b = a + b;
	if (IfFunc(node, 2))
		b = a - b;
	if (IfFunc(node, 3))
		b = a * b;
	if (IfFunc(node, 4))
		b = a / b;
	if (IfFunc(node, 5))
		b = exp(log(a) * b);

	if ((node -> data.type == TYPEFUNCTION) && (node -> data.value >= 6))
		b = functions[(int) node -> data.value - 6].func(b);

	treeDestruct(node);
	node = CreateNode(TYPENUMBER, b);
	return node;
}

#define OPTFUNCNUMBER(pos1, pos2, func, num)								\
		if ((node -> pos1) && (node -> pos2) && IfFunc(node, func) && IfNum(node -> pos1, num))					\
		{																	\
			tree_node_t* temp = node -> pos2;								\
			node -> pos2 = nullptr;											\
			treeDestruct(node);												\
			(*counter)++;													\
			return temp;													\
		}
#define MULMULOPT(node1, node2, node3, op1, op2)													\
		if ((node1) && (node2) && (node3) && IfFunc(node, op1) && IfFunc(node1, op2) && IfNumber(node3) && IfNumber(node2) )			\
		{																							\
			double a = node2 -> data.value, b = node3 -> data.value;								\
			node3 -> data.value = a * b;															\
			node2 -> data.value = 1;																\
			(*counter)++;																			\
		}
#define SUMSUMOPT(node1, node2, node3, op1, op2)													\
		if ((node1) && (node2) && (node3) && IfFunc(node, op1) && IfFunc(node1, op2) && IfNumber(node3) && IfNumber(node2) )			\
		{																							\
			double a = node2 -> data.value, b = node3 -> data.value;								\
			node3 -> data.value = a + b;															\
			node2 -> data.value = 0;																\
			(*counter)++;																			\
			return node;																			\
		}
#define DIVDIVOPT(node1, node2, node3, op1, op2)													\
		if ((node1) && (node2) && (node3) && IfFunc(node, op1) && IfFunc(node1, op2) && IfNumber(node3) && IfNumber(node2) )			\
		{																							\
			double a = node2 -> data.value, b = node3 -> data.value;								\
			node3 -> data.value = a / b;															\
			node2 -> data.value = 1;																\
			(*counter)++;																			\
			return node;																			\
		}
#define SUBSUBOPT(node1, node2, node3, op1, op2)													\
		if ((node1) && (node2) && (node3) && IfFunc(node, op1) && IfFunc(node1, op2) && IfNumber(node3) && IfNumber(node2) )			\
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

	if (IfFunc(node, 24))
		node = Differencial(node -> right, (int) node -> left -> data.value);

	if ((node -> left != nullptr) && (node -> left -> data.type != TYPENUMBER))
		node -> left = treeNodeOptimize(node -> left, counter);
	if ((node -> right != nullptr) && (node -> right -> data.type != TYPENUMBER))
		node -> right= treeNodeOptimize(node -> right, counter);

	if (IfFunc(node, 3) && (IfNum(node -> left, 0) || IfNum(node -> right, 0)))
	{
		treeDestruct(node);
		node = CreateNode(TYPENUMBER, 0);
		(*counter)++;
		return node;
	}
	OPTFUNCNUMBER(left, right, 3, 1);
	OPTFUNCNUMBER(right, left, 3, 1);
	OPTFUNCNUMBER(right, left, 4, 1);
	OPTFUNCNUMBER(left, right, 1, 0);
	OPTFUNCNUMBER(right, left, 1, 0);
	OPTFUNCNUMBER(right, left, 2, 0);
	OPTFUNCNUMBER(right, left, 5, 1);
	if (((node -> right)) && (node -> data.type == TYPEFUNCTION) && (!(node -> left) || (node -> left -> data.type == TYPENUMBER)) && (node -> right -> data.type == TYPENUMBER))
	{
		(*counter)++;
		return treeSolve(node);
	}
	MULMULOPT(node -> left, node -> right, node -> left -> left, 3, 3);
	MULMULOPT(node -> left, node -> right, node -> left -> right, 3, 3);//
	MULMULOPT(node -> right, node -> left, node -> right -> left, 3, 3);//
	MULMULOPT(node -> right, node -> left, node -> right -> right, 3, 3);//
	MULMULOPT(node -> left, node -> right, node -> left -> left, 3, 4);//
	MULMULOPT(node -> right, node -> left, node -> right -> left, 3, 4);//
	MULMULOPT(node -> right, node -> right -> right, node -> left, 4, 4);
	SUMSUMOPT(node -> left, node -> right, node -> left -> left, 1, 1);
	SUMSUMOPT(node -> left, node -> right, node -> left -> right, 1, 1);
	SUMSUMOPT(node -> right, node -> left, node -> right -> left, 1, 1);
	SUMSUMOPT(node -> right, node -> left, node -> right -> right, 1, 1);
	SUMSUMOPT(node -> right, node -> right -> right, node -> left, 2, 2);
	SUMSUMOPT(node -> left, node -> right, node -> left -> right, 2, 2);
	DIVDIVOPT(node -> left, node -> right, node -> left -> right, 4, 4);
	DIVDIVOPT(node -> right, node -> left, node -> right -> right, 4, 4);
	if ((node -> left) && (node -> right) && (node -> left -> right) && IfFunc(node, 3) && IfFunc(node -> left, 4) && IfNumber(node -> right) && IfNumber(node->left->right) )
		{
			double a = node -> right -> data.value, b = node->left->right -> data.value;
			node->left->right -> data.value = b / a;
			node -> right -> data.value = 1;
			(*counter)++;
			return node;
		}
	if ((node -> left) && (node -> right) && (node -> left -> right) && IfFunc(node, 4) && IfFunc(node -> left, 3) && IfNumber(node -> right) && IfNumber(node->left->right) )
		{
			double a = node -> right -> data.value, b = node->left->right -> data.value;
			node->right -> data.value = a / b;
			node -> left -> right -> data.value = 1;
			(*counter)++;
			return node;
		}
	SUBSUBOPT(node -> left, node -> left -> right, node -> right, 2, 1);
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

//=============================================================================================================


tree_node_t* GetTree(const char* str)
{
    s = str;
    starts = s;

    return GetGeneral();
}

tree_node_t* GetGeneral()
{
    //printf("GetGeneral\n");
    while(((*s) == '\n') || ((*s) == ' '))
            s++;

    tree_node_t* val = nullptr;
    //printf("/%s/ %d\n", s, *s);
    if(!(iscntrl(*s) && (!isspace(*s))))
    {
        val = CreateNode(TYPEOPERATOR, SEMICOLONop);

        val -> left = GetMainCode();

        while(((*s) == '\n') || ((*s) == ' '))
            s++;

        val -> right = GetGeneral();
    }
    return val;
}


tree_node_t* GetMainCode()
{
    //printf("GetMain\n");
    int n = 0;
    char text[MAXCOMMANDLEN] = "";
    const char * s1 = s;

    sscanf(s1, "%[^ \t\n(),=><!/*+^/-]%n", text, &n);
    s1 += n;

    while(*s1 == ' ')
        s1++;

    if (*s1 == '=')
        return GetAsign();

    return GetFunc();
}

tree_node_t* GetAsign()
{
    //printf("GetAsign\n");
    tree_node_t* val = CreateNode(TYPEOPERATOR, ASSIGNop);

    val -> left = GetId();

    SkipSpaces();
    s++;
    SkipSpaces();

    val -> right = GetG();
    return val;
}

tree_node_t* GetFunc()
{
    //printf("GetFunc\n");
    tree_node_t* val = GetId();

    while ((*s) == '\n')
        s++;

    val -> right = GetOp(1);

    return val;
}

tree_node_t* GetOp(int count)
{
    //printf("GetOp\n");
    if ((*(s + count - 1)) != '\t')
        return nullptr;
    s += count;

    tree_node_t* val = CreateNode(TYPEOPERATOR, OPop);
    val -> left = GetObject(count);

    SkipSpaces();
    while ((*s) == '\n')
        s++;

    val -> right = GetOp(count);

    return val;
}

tree_node_t* GetObject(int count)
{
    //printf("GetObject\n");
    int n = 0;
    char text[MAXCOMMANDLEN] = "";
    const char * s1 = s;
    sscanf(s1, "%[^ \t\n(),=><!/*+^/-]%n", text, &n);

    s1 += n;
    while(*s1 == ' ')
        s1++;

    if (strcmp(text, "if") == 0)
    {
        s += n;
        return GetIf(count);
    }

    if (strcmp(text, "while") == 0)
    {
        s += n;
        return GetWhile(count);
    }

    if (strcmp(text, "while") == 0)
    {
        s += n;
        return GetWhile(count);
    }

    if (strcmp(text, "ret") == 0)
    {
        tree_node_t* val = CreateNode(TYPEOPERATOR, RETURNop);
        s += n;
        SkipSpaces();

        if ((*s) != '\n')
            val -> left = GetG();
        else 
            s++;

        return val;
    }

    if (*s1 == '=')
        return GetAsign();

    return GetId();
}

tree_node_t* GetIf(int count)
{
    //printf("GetIf\n");
    SkipSpaces();

    if ((*s) != '(')
        printERROR("Something missed here, maybe \'(\'");

    s++;
    SkipSpaces();

    tree_node_t* val = CreateNode(TYPEOPERATOR, IFop, nullptr, CreateNode(TYPEOPERATOR, IFELSEop));
    val -> left = GetComp();

    SkipSpaces();
    if ((*s) != ')')
        printERROR("Something missed here, maybe \')\'");
    s++;
    SkipSpaces();

    while ((*s) == '\n')
        s++;

    val -> right -> left = GetOp(count + 1);

    int n = 0;
    char text[MAXCOMMANDLEN] = "";
    const char * s1 = s + count;
    sscanf(s1, "%[^ \t\n(),=><!/*+^/-]%n", text, &n);

    if (strcmp(text, "else") != 0)
        return val;

    s += count + n;
    SkipSpaces();
    while ((*s) == '\n')
        s++;

    val -> right -> right = GetOp(count + 1);

    return val;
}

tree_node_t* GetComp()
{
    //printf("GetComp\n");
    tree_node_t* val = GetExpression();

    SkipSpaces();
    int n = 0;
    char text[MAXCOMMANDLEN] = "";

    sscanf(s, "%[=><!]%n", text, &n);

    s += n;
    SkipSpaces();

    if ((n > 2) || (n == 0))
        printERROR("what means this comparating");

    val = CreateNode(TYPECOMPARE, CompChoose(text), val, nullptr);


    val -> right = GetExpression();

    return val;
}

tree_node_t* GetWhile(int count)
{
    //printf("GetWhile\n");
    SkipSpaces();

    if ((*s) != '(')
        printERROR("Something missed here, maybe \'(\'");

    s++;
    SkipSpaces();

    tree_node_t* val = CreateNode(TYPEOPERATOR, WHILEop);

    val -> left = GetComp();

    SkipSpaces();

    if ((*s) != ')')
        printERROR("Something missed here, maybe \')\'");

    s++;
    SkipSpaces();

    while ((*s) == '\n')
        s++;

    val -> right = GetOp(count + 1);

    return val;
}

tree_node_t* GetArg()
{
    //printf("GetArg\n");
    tree_node_t* val = CreateNode(TYPEOPERATOR, COMMAop);

    val -> right = GetG();

    SkipSpaces();

    if ((*s) == ',')
    {
        s++;

        val -> left = GetArg();
    }

    return val;
}


tree_node_t* GetG()
{
    //printf("GetG\n");

    tree_node_t* val = GetExpression();

    SkipSpaces();

    if (!((*s == '\n') || (*s == '\0') || (*s == ',') || (*s == ')')))
        printERROR("the last symbol of expression is no a \'\\n\', \',\', \'\\0\', \')\'");

    return val;
}

tree_node_t* GetExpression()
{
    //printf("GetExp\n");

    tree_node_t* node = GetT();
    
    SkipSpaces();

    while (*s == '+' || *s == '-')
    {
        char OP = *s;

        s++;

        tree_node_t* node2 = GetT();

        if (OP == '+')
            node = CreateNode(TYPEFUNCTION, functionlist("+"), node, node2);
        else
            node = CreateNode(TYPEFUNCTION, functionlist("-"), node, node2);
    }

    return node;
}

tree_node_t* GetT()
{
    //printf("GetT\n");

    tree_node_t* node = GetK();

    while (*s == '*' || *s == '/')
    {
        char OP = *s;

        s++;

        tree_node_t* node2 = GetK();

        if (OP == '*')
            node = CreateNode(TYPEFUNCTION, functionlist("*"), node, node2);
        else
            node = CreateNode(TYPEFUNCTION, functionlist("/"), node, node2);
    }

    return node;
}

tree_node_t* GetK()
{
    //printf("GetK\n");
    tree_node_t* node = GetP();

    SkipSpaces();

    if (*s == '^')
    {
        s++;

        tree_node_t* node2 = GetP();

        node = CreateNode(TYPEFUNCTION, 5, node, node2);
    }

    return node;
}

tree_node_t* GetP()
{
    //printf("GetP\n");
    SkipSpaces();

    if (*s == '(')
    {
        s++;

        tree_node_t* node = GetExpression();

        SkipSpaces();

        if (*s != ')')
        {
            printERROR("missing \')\'");
        }

        s++;

        return node;
    }

    if (isdigit(*s))
        return GetNumber();

    return GetId();
}

tree_node_t* GetNumber()
{   
    //printf("GetN\n");
    double val = 0;
    int n = 0;

    sscanf( s,"%lf%n", &val, &n);

    s += n;
    SkipSpaces();

    return CreateNode(TYPENUMBER, val);
}

tree_node_t* GetId()
{
    //printf("GetId\n");
    char func[FUNCTIONMAXLEN] = {};
    int n = 0;

    SkipSpaces();

    sscanf(s, "%[^ \t\n(),=><!/*+^/-]%n", func, &n);

    s += n;
    SkipSpaces();

    tree_node_t* node = nullptr;

    if((*s) == '(')
    {
        if (functionlist(func) < 22)
            node = CreateNode(TYPEFUNCTION, functionlist(func), nullptr, GetP());

        else if (functionlist(func) < 24)
            node = CreateNode(TYPEFUNCTION, functionlist(func), GetP(), nullptr);

        else if (functionlist(func) == 24)
        {
            node = CreateNode(TYPEFUNCTION, functionlist(func));

            s++;

            node -> left = GetId();

            if ((*s) != ',')
                printERROR("Missing \',\'");

            s++;

            node -> right = GetG();

            if ((*s) != ')')
                printERROR("Missing \')\'");

            s++;
        }
        else
        {
            node = CreateNode(TYPEFUNCTION, functionlist(func));

            s++;

            SkipSpaces();

            if ((*s) != ')')
                node -> left = GetArg();

            if ((*s) == ')')
                s++;
        }

    }
    else
        node = CreateNode(TYPEVARIABLE, findVariable(func));

    return node;
}

void DeleteComments(char *code)
{
    char* str = code;

    while((*str) != '\0')
    {
        if ((*str) == '@')
        {
            *str = ' ';

            while ((*str) != '@')
            {
                *str = ' ';

                str++;
            }

            *str = ' ';
        }

        str++;
    }

    char * last = code;
    str = code;

    while((*str) != '\0')
    {
        if ((*str) > ' ')
            last = str;

        str++;
    }

    *(last + 1) = '\0';

}

void SkipSpaces()
{
    while(*s == ' ')
        s++;
}

void printERROR(const char *text)
{
    if (s - starts >= 8)
        printf("Syntax Error: %s\n\t%-.8s\n\t\t^\n\t\t%lu\n", text, s - 5, s - starts);

    else
        printf("Syntax Error: %s\n\t%-.8s\n\t   ^^^^^^\n\t     %lu\n", text, starts, s - starts);
    
    fflush(0);
    abort();
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
    variables[number_of_var].numnum = 100;

    number_of_var++;

    return number_of_var - 1;
}

void _MakeVarList(tree_node_t* node)
{
    static int VarVar = 0;

    if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == SEMICOLONop) && ((node -> left -> data.type) == TYPEOPERATOR) && ((node -> left -> data.value) == ASSIGNop))
        if ((node -> left -> left -> data.type) == TYPEVARIABLE)
            variables[(int) node -> left -> left -> data.value].num = 0;

    if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == SEMICOLONop) && ((node -> left -> data.type) == TYPEFUNCTION))
        VarVar = node -> left -> data.value - 24;

    if ((node -> data.type) == TYPEVARIABLE)
        if ((variables[(int) node -> data.value].num != VarVar) && (variables[(int) node -> data.value].num != 0))
            node -> data.value = findVariable(variables[(int) node -> data.value].name, VarVar);

    if ((node -> left) != nullptr)
        _MakeVarList(node -> left);

    if ((node -> right) != nullptr)
        _MakeVarList(node -> right);
}

void _MakeVarList_for_function(tree_node_t* node)
{
    static int VarVar = 0;

    if ((node -> left != nullptr) && (((node -> left -> data.type) == TYPEFUNCTION) && ((node -> left -> left) != nullptr)))
    {
        VarVar = 0;
        _MakeVarList_for_function(node -> left-> left);
    }
    else if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == COMMAop))
    {
        variables[(int)node -> right -> data.value].numnum = VarVar;
        VarVar++;
        if ((node -> left) != nullptr)
            _MakeVarList_for_function(node -> left);
    }
    else if ((node -> right) != nullptr)
    {
        _MakeVarList_for_function(node -> right);
    }
}

int comp(const void* a, const void* b)
{
    if ((((Namenumnum_t*) a) -> num) != (((Namenumnum_t*) b) -> num))
        return ((Namenumnum_t*) a) -> num - ((Namenumnum_t*) b) -> num;

    return ((Namenumnum_t*) a) -> numnum - ((Namenumnum_t*) b) -> numnum;
}

void MakeVarList(tree_node_t* node)
{

    _MakeVarList(node);

    _MakeVarList_for_function(node);

    qsort(variables, number_of_var, sizeof(Namenumnum_t), comp);

    int counter = -1;
    int counter_ = 1;
    for (int i = 0; i < number_of_var; i++)
        if (variables[i].num == counter)
        {
            variables[i].numnum = counter_;
            counter_++;
        }
        else
        {
            if (counter >= 0)
                VarNumberArray[counter] = counter_ - 1;
            counter++;
            counter_ = 1;
            i--;
        }
    VarNumberArray[counter] = counter_ - 1;

    for (int i = 0; i <= counter; i++)
        printf("%d) %d\n", i, VarNumberArray[i]);
             
}

int FindVarNumber(int count)
{
    for (int i = 0;;i++)
        if (variables[ count ].num == variables[ i ].num)
        {
            return variables[ count ].numnum;
            break;
        }
    return 0;
}

int FindBasePointer(int count)
{
    for (int i = 0;;i++)
        if (variables[ count ].num == variables[ i ].num)
        {
            return i;
            break;
        }
    return 0;
}

//************************************************************************************************************************************************************************************************************************************************

void GraphPrintTree(FILE *dump, tree_node_t* headNode)
{
    assert(dump);

    if (headNode -> data.type == TYPENUMBER)
        fprintf(dump, "\"%p\"[shape = egg, color = \"darkseagreen\", label = \"%lg\"];\n", headNode, headNode -> data.value);

    else if (headNode -> data.type == TYPEVARIABLE)
        fprintf(dump, "\"%p\"[shape = egg, color = \"darkolivegreen1\", label = \"%s\"];\n", headNode, variables[(int)headNode -> data.value].name);

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value < 6))
        fprintf(dump, "\"%p\"[shape = diamond, color = \"gold\", label = \"%s\"];\n", headNode, takefunc((int) headNode -> data.value));

    else if (headNode -> data.type == TYPEFUNCTION)
        fprintf(dump, "\"%p\"[shape = box, color = \"grey\", label = \"%s%s\"];\n", headNode, (headNode -> data.value > 24)? "$" : "", takefunc((int) headNode -> data.value));

    else if (headNode -> data.type == TYPEOPERATOR)
        fprintf(dump, "\"%p\"[shape = %s, color = \"%s\", label = \"%s\"];\n", headNode, operators[(int) headNode -> data.value].form, operators[(int) headNode -> data.value].color, operators[(int) headNode -> data.value].name);

    else
        fprintf(dump, "\"%p\"[shape = parallelogram, color = \"grey56\", label = \"%s\"];\n", headNode, compare_operators[(int) headNode -> data.value].name);

    if(headNode -> left !=  nullptr)
    {
        fprintf(dump, "\"%p\" -> \"%p\";\n", headNode, headNode -> left);

        GraphPrintTree(dump, headNode -> left);
    }

    if(headNode -> right !=  nullptr)
    {
        fprintf(dump, "\"%p\" -> \"%p\";\n", headNode, headNode -> right);

        GraphPrintTree(dump, headNode -> right);
    }
}

void GraphTreeDump(tree_node_t* headNode)
{
    assert(headNode);

    static unsigned int DumpNumber = 0;

    char FileIn[17] = "";
    snprintf(FileIn, sizeof(FileIn), "files/Dump%02d.dot", DumpNumber);
    FILE* dump = fopen(FileIn, "wb");
    assert(dump);

    fprintf(dump, "digraph listDump\n{\n\n");

    GraphPrintTree(dump, headNode);
    
    fprintf(dump, "}");
    fclose(dump);

    Systemf("dot -Tpdf  files/Dump%02d.dot -o files/Dump%02d.pdf", DumpNumber, DumpNumber);
    Systemf("rm files/Dump%02d.dot", DumpNumber);
    //Systemf("evince Dump%02d.pdf", DumpNumber);

    DumpNumber++;    
}

void WriteTree(FILE *Write, tree_node_t* headNode)
{
    assert(headNode);
    assert(Write);

    fprintf(Write, "{");

    if (headNode -> data.type == TYPENUMBER)
        fprintf(Write, "%lg", headNode -> data.value);

    else if (headNode -> data.type == TYPEVARIABLE)
        fprintf(Write, "%s", variables[(int)headNode -> data.value].name);

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value <= 24))
        fprintf(Write, "%s", takefunc((int) headNode -> data.value));

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value > 24))
        fprintf(Write, "$%s", takefunc((int) headNode -> data.value)); 

    else if (headNode -> data.type == TYPEOPERATOR)
        fprintf(Write, "%s", operators[(int) headNode -> data.value].name);   

    else
        fprintf(Write, "%s", compare_operators[(int) headNode -> data.value].name);


    if((headNode -> left !=  nullptr) || (headNode -> right !=  nullptr))
    {
        if(headNode -> left !=  nullptr)
            WriteTree(Write, headNode -> left);

        else
            fprintf(Write, "@");

        if(headNode -> right !=  nullptr)
            WriteTree(Write, headNode -> right);

        else
            fprintf(Write, "@");
    }

    fprintf(Write, "}");
}

void preWriteTree(const char *FileOut, tree_node_t* headNode)
{
    assert(headNode);

    FILE* Write = fopen(FileOut, "w");
    assert(Write);

    WriteTree(Write, headNode);
    
    fclose(Write);
}

tree_node_t* readTree()
{
    tree_node_t* node = nullptr;

    if ((*s) == '@')
    {
        s++;
        return node;
    }

    if ((*s) == '{')
    {
        s++;

        char text[MAXCOMMANDLEN] = "";
        int i = 0;

        sscanf(s, " %[^{}@ ] %n", text, &i);
        s += i;

        if (strcmp(";", text) == 0)
            node = CreateNode(TYPEOPERATOR, SEMICOLONop);

        else if (strcmp(",", text) == 0)
            node = CreateNode(TYPEOPERATOR, COMMAop);

        else if (strcmp("if", text) == 0)
            node = CreateNode(TYPEOPERATOR, IFop);

        else if (strcmp("if-else", text) == 0)
            node = CreateNode(TYPEOPERATOR, IFELSEop);

        else if (strcmp("while", text) == 0)
            node = CreateNode(TYPEOPERATOR, WHILEop);

        else if (strcmp("op", text) == 0)
            node = CreateNode(TYPEOPERATOR, OPop);

        else if (strcmp("ret", text) == 0)
            node = CreateNode(TYPEOPERATOR, RETURNop);

        else if (strcmp("=", text) == 0)
            node = CreateNode(TYPEOPERATOR, ASSIGNop);

        else if (isdigit(text[0]))
            node = CreateNode(TYPENUMBER, atof(text));

        else if (strcmp("==", text) == 0)
            node = CreateNode(TYPECOMPARE, 0);

        else if (strcmp("!=", text) == 0)
            node = CreateNode(TYPECOMPARE, 1);

        else if (strcmp(">", text) == 0)
            node = CreateNode(TYPECOMPARE, 2);

        else if (strcmp(">=", text) == 0)
            node = CreateNode(TYPECOMPARE, 3);

        else if (strcmp("<", text) == 0)
            node = CreateNode(TYPECOMPARE, 4);

        else if (strcmp("<=", text) == 0)
            node = CreateNode(TYPECOMPARE, 5);

        else if (text[0] == '$')
            node = CreateNode(TYPEFUNCTION, functionlist(text + 1));

        else if ((*s) == '}')
            node = CreateNode(TYPEVARIABLE, findVariable(text));

        else 
            node = CreateNode(TYPEFUNCTION, functionlist(text));

        node -> left = readTree();

        node -> right = readTree();

        if ((*s) == '}')
            s++;
    }

    return node;  
}

tree_node_t* readTreeFrom(char* input)
{
    assert(input);

    s = input;

    return readTree();
}

const char* ReplaceCode(const char* incode, int num)
{
    for (int i = 0; i < COMMANDSNUMBER; i++)
    {
        if (num == 1)
        {
            if ( strcmp (MyLangCommands[i].MyLang, incode) == 0 )
                return MyLangCommands[i].CLang;
        }

        else
            if ( strcmp (MyLangCommands[i].CLang, incode) == 0 )
                return MyLangCommands[i].MyLang;
    }

    return incode;
}

char* ReCode(char* input, int num)
{
    char* outcode = (char*) calloc(strlen(input), sizeof(outcode[0]));

    s = input;

    while ((*s) != '\0')
    {

        if ( isspace(*s) || ispunct(*s))
        {
            outcode[strlen(outcode)] = *s;

            s++;
        }
        else
        {
            int n = 0;
            char text[MAXCOMMANDLEN] = "";
            sscanf(s, "%[^ \t\n(),=><!/*+^/-]%n", text, &n);

            s += n;

            strcat(outcode, ReplaceCode(text, num));
        }
    }

    return outcode;
}

void WriteTreeLang(FILE *Write, tree_node_t* headNode, int tabs = 0)
{
    assert(headNode);
    assert(Write);

    if (headNode -> data.type == TYPENUMBER)
        fprintf(Write, "%lg", headNode -> data.value);

    else if (headNode -> data.type == TYPEVARIABLE)
        fprintf(Write, "%s", variables[(int)headNode -> data.value].name);

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value < 3))
    {
        fprintf(Write, "(");
        WriteTreeLang(Write, headNode -> left);
        fprintf(Write, " %s ", ReplaceCode(takefunc((int) headNode -> data.value), 0));
        WriteTreeLang(Write, headNode -> right);
        fprintf(Write, ")");
    }

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value < 5))
    {
        WriteTreeLang(Write, headNode -> left);
        fprintf(Write, " %s ", ReplaceCode(takefunc((int) headNode -> data.value), 0));
        WriteTreeLang(Write, headNode -> right);
    }

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value == 5))
    {
        fprintf(Write, "(");
        WriteTreeLang(Write, headNode -> left);
        fprintf(Write, ")");
        fprintf(Write, " ^ ");
        fprintf(Write, "(");
        WriteTreeLang(Write, headNode -> right);
        fprintf(Write, ")");
    }

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value < 22))
    {
        fprintf(Write, "%s ( ", ReplaceCode(takefunc((int) headNode -> data.value), 0));
        WriteTreeLang(Write, headNode -> right);
        fprintf(Write, " )");
    }

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value < 24))
    {
        fprintf(Write, "%s ( ", ReplaceCode(takefunc((int) headNode -> data.value), 0));
        WriteTreeLang(Write, headNode -> left);
        fprintf(Write, " )");
    }

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value == 24))
    {
        fprintf(Write, "%s ( ", ReplaceCode(takefunc(24), 0));
        WriteTreeLang(Write, headNode -> left);
        fprintf(Write, ", ");
        WriteTreeLang(Write, headNode -> right);
        fprintf(Write, " )");
    }

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value > 24))
    {
        fprintf(Write, "%s( ", ReplaceCode( takefunc((int) headNode -> data.value), 0));
        
        if ((headNode -> left) != nullptr)
            WriteTreeLang(Write, headNode -> left);

        fprintf(Write, " )");
        
        if ((headNode -> right) != nullptr)
            WriteTreeLang(Write, headNode -> right, 1);
    }

    else if (headNode -> data.type == TYPEOPERATOR)
        switch((int) headNode -> data.value)
        {
            case OPop:
                fprintf(Write, "\n");

                for (int i = 0; i < tabs; i++)
                    fprintf(Write, "\t");

                WriteTreeLang(Write, headNode -> left, tabs);

                if ((headNode -> right) != nullptr)
                    WriteTreeLang(Write, headNode -> right, tabs);

                break;

            case SEMICOLONop:
                WriteTreeLang(Write, headNode -> left, 0);

                fprintf(Write, "\n\n");

                if ((headNode -> right) != nullptr)
                    WriteTreeLang(Write, headNode -> right, 0);

                break;

            case COMMAop:
                WriteTreeLang(Write, headNode -> right);

                if ((headNode -> left != nullptr) && (headNode -> left -> data.type == TYPEOPERATOR) && (headNode -> left -> data.value == COMMAop))
                {
                    fprintf(Write, ", ");

                    WriteTreeLang(Write, headNode -> left, 0);
                }
                break;

            case WHILEop:
                fprintf(Write, "%s (", ReplaceCode("while", 0));
                
                WriteTreeLang(Write, headNode -> left);
                
                fprintf(Write, " )");
                
                WriteTreeLang(Write, headNode -> right, tabs + 1);
                
                break;

            case IFop:
                fprintf(Write, "%s ( ", ReplaceCode("if", 0));
                
                WriteTreeLang(Write, headNode -> left);
                
                fprintf(Write, " )");
                
                WriteTreeLang(Write, headNode -> right, tabs + 1);
                
                break;

            case IFELSEop:
                WriteTreeLang(Write, headNode -> left, tabs);
                
                if  ((headNode -> right) != 0) 
                {
                    fprintf(Write, "\n");
                    
                    for (int i = 0; i < tabs - 1; i++)
                        fprintf(Write, "\t");
                    
                    fprintf(Write, "%s", ReplaceCode("else", 0));
                    
                    WriteTreeLang(Write, headNode -> right, tabs);
                }

                break;

            case RETURNop:
                fprintf(Write, "%s ", ReplaceCode("ret", 0)); 
                
                if (headNode -> left != nullptr)
                    WriteTreeLang(Write, headNode -> left);
                
                fprintf(Write, "\n"); 
                
                break;

            case ASSIGNop:
                WriteTreeLang(Write, headNode -> left);
                
                fprintf(Write, " %s ", ReplaceCode("=", 0));
                
                WriteTreeLang(Write, headNode -> right);
                
                break;
    }
    else if (headNode -> data.type == TYPECOMPARE)
    {
        WriteTreeLang(Write, headNode -> left);
        
        fprintf(Write, " %s ", ReplaceCode(compare_operators[(int) headNode -> data.value].name, 0)); 
        
        WriteTreeLang(Write, headNode -> right);
    }
}

void WriteTreeToLang(const char *FileOut, tree_node_t* headNode)
{
    assert(headNode);

    FILE* Write = fopen(FileOut, "w");
    assert(Write);

    WriteTreeLang(Write, headNode);
    
    fclose(Write);
}


#define ASSEMBLERFUNC( number, name)\
                                            printf(#name"\n");                      \
                                            _toassembler(file, node -> left);       \
                                            _toassembler(file, node -> right);      \
                                            fprintf(file, #name"\n");

void _toassembler_comma(FILE* file, tree_node_t* node, int FuncNum, int num)
{
    _toassembler(file, node -> right);

    fprintf(file, "pop [ dx + %d ]\n", num + FuncNum);
    if ((node -> left) != nullptr)
        _toassembler_comma(file, node -> left, FuncNum, num + 1);
}

void _toassembler(FILE* file, tree_node_t* node)                                    //печать в ассемблер по ноде
{

    static int Number_Of_Labels = 0;
    static int FunctionNumber = 0;

    if ( ((node -> data.type) == TYPEOPERATOR) && ((((node -> data.value) == SEMICOLONop) && ((node -> left -> data.value) == ASSIGNop)) || ((node -> data.value) == OPop)) )
    {
        printf("SEMICOLON OP\n");fflush(0);
        _toassembler(file, node -> left);
        if ((node -> right) != nullptr)
            _toassembler(file, node -> right);
    }
    
    else if (IfFunc(node, 22))//get
    {
        printf("get\n");fflush(0);
        fprintf(file, "in\npop[ %s%d ]\n", (variables[(int) node -> left -> data.value].num > 0)? "dx + " : "", FindVarNumber( (int) node -> left -> data.value ));
    }

    else if (IfFunc(node, 23))//put
    {
        printf("put\n");fflush(0);
        _toassembler(file, node -> left);
        fprintf(file, "out\n");
    }

    else if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == ASSIGNop))
    {
        printf("assign\n");fflush(0);
        _toassembler(file, node -> right);
        fprintf(file, "pop [ %s%d ]\n", (variables[(int) node -> left -> data.value].num > 0)? "dx +  " : "", FindVarNumber( (int) node -> left -> data.value ));
    }
    else if (IfFunc(node, 1))
    {
        ASSEMBLERFUNC(1, add)
    }
    else if (IfFunc(node, 2))
    {
        ASSEMBLERFUNC(2, sub)
    }
    else if (IfFunc(node, 3))
    {
        ASSEMBLERFUNC(3, mul)
    }
    else if (IfFunc(node, 4))
    {
        ASSEMBLERFUNC(4, div)
    }
    else if (IfFunc(node, 5))
    {
        ASSEMBLERFUNC(5, pow)
    }
    else if (IfFunc(node, 21))
    {
        printf("sqrt\n");
        _toassembler(file, node -> right);
        fprintf(file, "sqrt\n");
    }
    else if ( (node -> data.type) == TYPEVARIABLE)
    {
        printf("var\n");fflush(0);
        fprintf(file, "push [ %s%d ]\n", (variables[(int) node -> data.value].num > 0)? "dx + " : "", FindVarNumber( (int) node -> data.value ));
    }

    else if (IfNumber(node))
    {
        printf("number\n");fflush(0);
        fprintf(file, "push %lg\n", node -> data.value);
    }

    else if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == IFop))
    {
        printf("if\n");fflush(0);
        int LabelNumber = Number_Of_Labels;
        Number_Of_Labels += 2;

        _toassembler(file, node -> left -> left);
        _toassembler(file, node -> left -> right);

        fprintf (file, "%s label%d:\n", JumpWords[(int)node -> left -> data.value].name, LabelNumber);

        _toassembler(file, node -> right -> left);

        if ((node -> right -> right) != nullptr)
        {
            fprintf(file, "jmp label%d:\nlabel%d:\n", LabelNumber + 1, LabelNumber);

            _toassembler(file, node -> right -> right);

            fprintf(file, "label%d:\n", LabelNumber + 1);
        }
        else
            fprintf(file, "label%d:\n", LabelNumber);
    }

    else if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == WHILEop))
    {
        printf("while\n");fflush(0);
        int LabelNumber = Number_Of_Labels;
        Number_Of_Labels += 2;

        fprintf(file, "label%d:\n", LabelNumber);

        _toassembler(file, node -> left -> left);
        _toassembler(file, node -> left -> right);

        fprintf (file, "%s label%d:\n", JumpWords[(int)node -> left -> data.value].name, LabelNumber + 1);

        _toassembler(file, node -> right);

        fprintf(file, "jmp label%d:\nlabel%d:\n", LabelNumber, LabelNumber + 1);
    }

    else if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == SEMICOLONop) && ((node -> left -> data.value) > 24))
    {
        printf("SEMICOLON FUNC\n");
        fflush(0);

        if (strcmp(takefunc((int)node -> left -> data.value), "main") == 0)
        {
            fprintf(file, "main:\n");
            fprintf(file, "push dx\npush %d\nadd\npop dx\n", number_of_var);
        }
        else
            fprintf(file, "func%d:\n", (int) node -> left -> data.value);

        FunctionNumber = node -> left -> data.value - 24;

        _toassembler(file, node -> left -> right);

        fprintf(file, "jmp endlabel:\n");
        
        if ((node -> right) != nullptr)
            _toassembler(file, node -> right);
    }
    else if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == RETURNop))
    {
        printf("ret %d\n", FunctionNumber);
        if (strcmp(takefunc(FunctionNumber + 24), "main") != 0)
        {
            _toassembler(file, node -> left);
            fprintf(file, "pop [ 0 ]\nret\n");
        }
    }
    else if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == COMMAop))
    {
        //-----------------------------------
    }
    else
    {
        if ((node -> left) != nullptr)
            _toassembler_comma(file, node -> left, VarNumberArray[FunctionNumber], 1);

        fprintf(file, "push dx\npush %d\nadd\npop dx\n", VarNumberArray[FunctionNumber]);
        fprintf(file, "call func%d:\n", (int)node -> data.value);
        fprintf(file, "push [ 0 ]\n");
        fprintf(file, "push dx\npush %d\nsub\npop dx\n", VarNumberArray[FunctionNumber]);
    }

}
#undef ASSEMBLERFUNC

void FromTreeToAssembler(const char *FileOut, tree_node_t* node)                        //печать в ассемблер
{
    assert(node);

    FILE* Write = fopen(FileOut, "w");
    assert(Write);
    fprintf(Write, "jmp main:\n");
    _toassembler(Write, node);
    fprintf(Write, "\n\nendlabel:\nend\n");
    
    fclose(Write);
}