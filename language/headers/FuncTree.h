#ifndef TREEFUNC_H
#define TREEFUNC_H

#include <stdio.h>
//#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <ctype.h>
#include "constants.h"


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

struct tree_node_t
{
    node_value_t data;
    tree_node_t* left;
    tree_node_t* right;
    tree_node_t* parant;
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

extern unit_t arr[MAXCODELEN];
extern int VarNumberArray[50];
extern Namenumnum_t variables[VARNUMBER];
extern int number_of_var;
extern const char * s;
extern const char * starts;

#endif
