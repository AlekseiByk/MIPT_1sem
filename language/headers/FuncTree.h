#ifndef TREEFUNC_H
#define TREEFUNC_H

#include <stdio.h>
//#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <ctype.h>


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

struct Namenumnum_t {
    char name[FUNCTIONMAXLEN];
    int num = -2;
    int numnum = 0;
};

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


const int MAXCODELEN = 10000;
const int VARNUMBER = 100;

tree_node_t* GetG();            // принимает формулу
tree_node_t* GetExpression();   // принимает формулу
tree_node_t* GetNumber();       // принимает число
tree_node_t* GetT();            // * или /
tree_node_t* GetK();            // ^
tree_node_t* GetE();            // + или -
tree_node_t* GetP();            // есть ли скобка
tree_node_t* GetId();           // переменная или функция

tree_node_t* GetTree(const char* str);  // главная 
tree_node_t* GetGeneral();              // основная
tree_node_t* GetMainCode();             // 
tree_node_t* GetAsign();                // присваивание
tree_node_t* GetFunc();                 // функция с параметрами и если есть операторами
tree_node_t* GetOp(int count);          // оператор
tree_node_t* GetObject(int count);      // объект
tree_node_t* GetIf(int count);          // сравнение
tree_node_t* GetWhile(int count);       // цикл
tree_node_t* GetArg();                  // аргументы функции
tree_node_t* GetComp();                 // аргументы сравнения

int findVariable(const char * val, int VarNum = -1);    // работа с переменными
void printERROR(const char *text);                      // распечатка синтаксической ошибки
void SkipSpaces();                                      // пропуск пробелов
tree_node_t* readTreeFrom(char* input);                 // читать дерево по стандарту
void DeleteComments(char *code);                        // удалить коментарии

void GraphTreeDump(tree_node_t* headNode);                              // графический дамп
void preWriteTree(const char *FileOut, tree_node_t* headNode);          // запись по стандарту
char* ReCode(char* input, int num);                                     // изменение от своего языка к си подобному
void WriteTreeToLang(const char *FileOut, tree_node_t* headNode);       // перево дерева в свой язык
void FromTreeToAssembler(const char *FileOut, tree_node_t* headNode);   // в код асемблера
void MakeVarList(tree_node_t* node);                                    // структурирует массив переменных
void _toassembler(FILE* file, tree_node_t* node);

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

const  struct MyCommands_t{
    char MyLang[MAXCOMMANDLEN];
    char CLang[MAXCOMMANDLEN];
} MyLangCommands[] = {
                        {"прымі","get"},
                        {"вывесці","put"},
                        {"калі","if"},
                        {"вярни","ret"},
                        {"інакш","else"},
                        {"пакуль","while"},
                        {"меньш","<"},
                        {"больш",">"},
                        {"роўны","=="},
                        {"раўно","="},
                        {"галоўная","main"},
                        {"вытворная","deriv"},
                        {"корань","sqrt"}
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


const Namenum_t JumpWords[] = { { "jne", 0}, { "je", 0}, { "jbe", 0}, { "jb", 0}, { "jae", 0}, { "ja", 0}};

const Namenum_t compare_operators[] = {	{ "==", 0}, { "!=", 0}, { ">", 0}, { ">=", 0}, { "<", 0}, { "<=", 0} };
const int compnum = 6;

const int COMMANDSNUMBER = 12;

extern unit_t arr[MAXCODELEN];
extern int VarNumberArray[50];
extern Namenumnum_t variables[VARNUMBER];
extern int number_of_var;
extern const char * s;
extern const char * starts;

#endif
