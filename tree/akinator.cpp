#include <stdio.h>
//#include <assert.h>
#include <stdlib.h>
#include "Text.h"
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <ctype.h>

#define MIN(arg1, arg2) (arg1 < arg2)? arg1 : arg2

#define ReText(waytonode, number)                                                            \
            k1 += printData(waytonode[number - i] -> data, "можно ", "нельзя ", 11);         \
            k1 += printData(waytonode[number - i] -> data, "меньше ", "больше ", 13);         \
            k1 += printData(waytonode[number - i] -> data, "больше ", "меньше ", 13);        \
            k1 += printData(waytonode[number - i] -> data, "нельзя ", "можно ", 13);        \
            k1 += printData(waytonode[number - i] -> data, "иногда ", "не ", 13);


typedef char element_t;
typedef int Size_t;

const char TREEFILE[] = "tree.txt";
const int MAXNODELEN = 1000;
const int MAXANSWERLEN = 10;
const int MAXTREELEN = 200;
const int MAXCOMMANDLEN = 200;
int VOICE = 1;

struct tree_node_t
{
    element_t* data;
    tree_node_t* left;
    tree_node_t* right;
    tree_node_t* parant;
};

void GraphTreeDump(tree_node_t* headNode);
tree_node_t* readTreeFrom(char* input,int length);
void preWriteTree(const char *FileOut, tree_node_t* headNode);
void akinatorPlay(tree_node_t* head);
void treeDestruct(tree_node_t* headNode);
tree_node_t* ShowWay(tree_node_t* headNode);
tree_node_t* SearchTreeNode(tree_node_t* headNode, char* data);
int CompairTwoWays(tree_node_t* headNode);
void voice(const char* format, ...);
void Systemf(const char* format, ...);

int main() 
{
	scanf("%d", &VOICE);
    int length = 0;
    char* input = TextFromFile(TREEFILE, &length);
    tree_node_t* head = readTreeFrom(input, length);
    free(input);
    preWriteTree(TREEFILE, head);

    int ans = 0;
    system("clear");
    voice("Привет! Я бинарный садовник,\n");
    while (true)
    {
        voice("Что желаэте выполнить?\n");
        printf("1>> Акинатор\n2>> Графический дамп\n3>> Узнать путь\n4>> Сравнить пути\n5>> Выход\n");
        scanf ("%d", &ans);
        system("clear");
        printf("Бинарный садовник:\n"); 
        switch(ans)
        {
            case 1:
            {   
                printf("1>> ");
                voice("Акинатор\n");
                voice("Теперь я Акинатор, Я могу угадать то, что ты загадал:\n");
                akinatorPlay(head);
                preWriteTree(TREEFILE, head);
                break;
            }
            case 2:
            {
                printf("2>> ");
                voice("Графический дамп\n");
                GraphTreeDump(head);
                voice("Графический дамп окончен\n");
                break;
            }
            case 3:
                printf("3>> ");
                voice("Показать путь\n");
                ShowWay(head);
                break;
            case 4:
                printf("4>> ");
                voice("Сравнить пути\n");
                CompairTwoWays(head);
                break;
            case 5:
                printf("5>> ");
                voice("Выход\nСпасибо за уделенное время\n");
                return 0;
            default:
                voice("неизвестная команда\n");
                fflush(stdin);
                break;
        }
        sleep(1);
    }
    treeDestruct(head);
    return 0;
}

void Systemf(const char* format, ...)
{
    va_list arg = {};
    va_start(arg, format);

    char systemCom[MAXCOMMANDLEN] = "";
    vsnprintf(systemCom, MAXCOMMANDLEN, format, arg);
    va_end(arg);
    system(systemCom);
}

void voice(const char* format, ...)
{
    va_list arg = {};
    va_start(arg, format);

    char txt[MAXCOMMANDLEN] = "";
    vsnprintf(txt, MAXCOMMANDLEN, format, arg);
    va_end(arg);

    printf("%s", txt);
    fflush(0);

    char *enter = txt;
    while(*enter != '\0')
    {
        if ( iscntrl(*enter) || ispunct(*enter) )
            *enter = ' ';
        enter++;
    }
    
    if (VOICE)
        Systemf("festival -b \'(begin (voice_msu_ru_nsh_clunits) (SayText \"%s\"))\'", txt);
}

tree_node_t* createNode(char* data)
{
    assert(data);
    tree_node_t* node = (tree_node_t*) calloc(1, sizeof(tree_node_t));
    assert(node);
    node -> data = strdup(data);
    //printf("%s\n", node -> data);                                  /*------------*/
    return node;
}

tree_node_t* readTreeFrom(char* input,int len)
{
    //printf("%d\n", len);
    assert(input);
    assert(len > 0);
    int i = 0;
    char* question= (char*) calloc(MAXNODELEN, sizeof(char));
    int scan = sscanf(input, "{ \"%[^\"]\" %n", question, &i);
    tree_node_t* temp1 = createNode(question);
    tree_node_t* head = temp1;
    //printf("%s %d %d\n", question, i, scan);                       /*------------*/
    int k = 0;

    while (i < len)
    {
        int n = 0;
        scan = sscanf(input + i, "{ \"%[^\"]\" %n", question, &n);
        //printf("%s %d %d\n", question, i, scan);                   /*------------*/
        
        if (scan == 1)
        {
            i += n;
            if (k == 0)
            {
                tree_node_t* temp = createNode(question);
                temp1 -> left = temp;
                temp -> parant = temp1;
                temp1 = temp;
            }
            else
            {
                tree_node_t* temp = createNode(question);
                temp1 -> right = temp;
                temp -> parant = temp1;
                temp1 = temp;
                k = 0;
            }
        }
        else 
        {
            if (input[i] == '}')
            {
                k = 1;
                i++;
                temp1 = temp1 -> parant;
            }
            else if (input[i] == '(')
            {
                k = 1;
                i += 5;
            }    
        }
        
    }  
    return head;  
}

void GraphPrintTree(FILE *dump, tree_node_t* headNode)
{
    //fprintf(dump, "\"%p\"[label = \"{ {%p |<data> %s} | {<left> %p|<right> %p}}\"];\n", headNode, headNode, headNode -> data, headNode -> left, headNode -> right);
    assert(dump);
    assert(headNode);

    int len = 0;
    while(headNode -> data[len] != 0)
            len++; 
    char* temp = (char*) calloc(MAXNODELEN * 10, sizeof(char));
    assert(temp);

    int k = 0;

    for (int i = 0; i < len; i++)
    {
        if (headNode -> data[i] == ' ')
        {
            temp[k++] = '&';
            temp[k++] = '#';
            temp[k++] = '9';
            temp[k++] = '2';
            temp[k++] = ';';
        }
        temp[k++] = headNode -> data[i];
    }

    fprintf(dump, "\"%p\"[label = \"{ <data> %s}\"];\n", headNode, temp);
    free(temp);

    if(headNode -> left !=  nullptr)
    {
        //fprintf(dump, "\"%p\":left -> \"%p\":data [label = \"Yes\"];\n", headNode, headNode -> left);
        fprintf(dump, "\"%p\":data -> \"%p\":data [label = \"Yes\"];\n", headNode, headNode -> left);
        GraphPrintTree(dump, headNode -> left);
    }
    if(headNode -> right !=  nullptr)
    {
        //fprintf(dump, "\"%p\":right -> \"%p\":data [label = \"No\"];\n", headNode, headNode -> right);
        fprintf(dump, "\"%p\":data -> \"%p\":data [label = \"No\"];\n", headNode, headNode -> right);
        GraphPrintTree(dump, headNode -> right);
    }
    if(headNode -> parant !=  nullptr)
    {
        fprintf(dump, "\"%p\":data -> \"%p\":data [color = \"red\"];\n", headNode, headNode -> parant);
    }
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
    Systemf("evince Dump%02d.pdf", DumpNumber);

    DumpNumber++;    
}

void WriteTree(FILE *Write, tree_node_t* headNode)
{
    //printf("%s %lu\n", headNode -> data, strlen(headNode -> data));       /*------------*/
    fprintf(Write, "{\"%s\"", headNode -> data);

    if(headNode -> left !=  nullptr || headNode -> right !=  nullptr)
    {
        if(headNode -> left !=  nullptr)
            WriteTree(Write, headNode -> left);
        else
            fprintf(Write, "(nil)");
        if(headNode -> right !=  nullptr)
            WriteTree(Write, headNode -> right);
        else
            fprintf(Write, "(nil)");
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

void akinatorPlay(tree_node_t* head)
{
    if (head -> left != nullptr)
    {
        voice("Это %s?\n", head -> data);
        char ans[MAXANSWERLEN] = "";
        do {
            scanf("%s", ans);
            if ((strcasecmp(ans, "да") == 0) || (strcasecmp(ans, "yes") == 0))
                akinatorPlay(head -> left);
            else if (strcasecmp(ans, "нет") == 0 || strcasecmp(ans, "no") == 0)
                akinatorPlay(head -> right);
            else
            {
                voice("я вас не понял\nПовторите ввод повторно\n");
                continue;
            }
            break;
        } while (true);
    }
    else
    {
        voice("Я думаю, что это %s, я прав?\n", head -> data);
        char ans[MAXANSWERLEN] = "";
        do {
            scanf("%s", ans);
            if (strcasecmp(ans, "да") == 0 || strcasecmp(ans, "yes") == 0)
                voice("Я же говорил\n");
            else if ((strcasecmp(ans, "нет") == 0) || (strcasecmp(ans, "no") == 0))
            {
                voice("А кто(что) это тогда?\n");
                char ans[MAXNODELEN] = {};
                scanf("\n%[^\n]", ans);
                head -> left = createNode(ans);
                head -> right = createNode(head -> data);
                head -> left -> parant = head;
                head -> right -> parant = head;
                voice("Чем %s отличается от %s?\n", head -> left -> data, head -> right -> data);
                scanf("\n%[^\n]", ans);

                free(head -> data);
                head -> data = strdup(ans);                                                           ///
                voice("Ваш ответ записан\n");
                fflush(0);
            }
            else
            {
                voice("Я вас не понял\nповторите ввод повторно\n");
                continue;
            }
            break;
        } while (true);
    }
}

void treeDestruct(tree_node_t* headNode)
{
    assert(headNode);
    assert(headNode -> data);

    free(headNode -> data);
    
    if(headNode -> left !=  nullptr)
        treeDestruct(headNode -> left);
    if(headNode -> right !=  nullptr)
        treeDestruct(headNode -> right);
    if (headNode != nullptr)
        free(headNode);
}

tree_node_t* SearchTreeNode(tree_node_t* headNode, char* data)
{
    if (headNode)
    {
        tree_node_t* temp = 0;
        if (strcasecmp(headNode -> data, data) == 0)
            return headNode;
        if((temp = SearchTreeNode(headNode -> left, data)))
            return temp;
        if((temp = SearchTreeNode(headNode -> right, data)))
            return temp;
    }
    return 0;
}

tree_node_t** ScanAndDoArray(tree_node_t* headNode, const char *text, int *number)
{
    char* ans= (char*) calloc(MAXNODELEN, sizeof(char));
    assert(ans);
    
    voice("%s\n", text);
    scanf("\n%[^\n]", ans);
    tree_node_t* node = SearchTreeNode(headNode, ans);
    free(ans);

    tree_node_t** waytonode = (tree_node_t**) calloc(MAXNODELEN, sizeof(waytonode[0]));
    if (!node)
    {
        voice("Íåèçâåñòíî, äëÿ îïåðåäëåíèÿ ïóòè ñíà÷àëà äîáàâüòå â äåðåâî\n");
        return 0;
    }
    while (node)
    {
            waytonode[*number] = node;
            (*number)++;
            node = node -> parant;
    }

    return waytonode;
}

int printData(char From[], const char *With, const char *paste, int len)
{
    if ((strncasecmp(From, With, len) == 0))
    {
        voice("%s%s ", paste, From + len);
        return 1;
    }
    return 0;
}

void PrintForCompair(tree_node_t** waytonode, int number, int k, const char* text = "")
{
    for (int i = k - 1; i < number; i++)
    {
        if (waytonode[number - i - 1] == waytonode[number - i] -> left)
            voice("%s ", waytonode[number - i] -> data);
        else 
        {
            int k1 = 0;
            ReText(waytonode, number);
            if (k1 == 0)
                voice("Íå %s ", waytonode[number - i] -> data);
        }
        printf("%s", text);
    }
}

tree_node_t* ShowWay(tree_node_t* headNode)
{
    int number = 0;
    tree_node_t** waytonode = ScanAndDoArray(headNode, "Введите то, чей путь хотите узнать:", &number);
    if (!waytonode)
        return 0;

    PrintForCompair(waytonode, number, 2, ">> ");

    voice("%s", waytonode[0] -> data);
    printf("\n");
    free(waytonode);
    return waytonode[0];
}

int CompairTwoWays(tree_node_t* headNode)
{
    int number1 = 0;
    tree_node_t** waytonode1 = ScanAndDoArray(headNode, "Введте первое из того, что хотите сравнить:", &number1);
    if (!waytonode1)
        return 0;
    
    int number2 = 0;
    tree_node_t** waytonode2 = ScanAndDoArray(headNode, "Введте второе из того, что хотите сравнить:", &number2);
    if (!waytonode2)
        return 0;

    printf("\n\n");

    if (waytonode1[0] == waytonode2[0])
    {
        voice("îíè îäèíàêîâû\n");
        return 0;
    }
    int i = 2;
    int k = 2;
    if (waytonode1[number1 - i] == waytonode2[number2 - i])
    {
        voice("%s è %s ïîõîæè òåì, ÷òî ", waytonode1[0] -> data, waytonode2[0] -> data);
        if (waytonode1[number1 - i] == waytonode1[number1 - i + 1] -> left)
            voice("%s", waytonode1[number1 - i + 1] -> data);
        else
        {
            int k1 = 0;
            ReText(waytonode1, number1 + 1);
            if (k1 == 0)
               voice("íå %s", waytonode1[number1 - i + 1] -> data);
        }
        i++;
        while (i < MIN(number1, number2))
        {
            if (waytonode1[number1 - i] == waytonode2[number2 - i])
            {
                if (waytonode1[number1 - i] == waytonode1[number1 - i + 1] -> left)
                    voice(", %s", waytonode1[number1 - i + 1] -> data);
                else
                {
                    int k1 = 0;
                    ReText(waytonode1, number1 + 1);
                    if (k1 == 0)
                        voice(", íå %s ", waytonode1[number1 - i + 1] -> data);
                }
            }
            else
            {
                k = i;
                voice("\nÀ òàêæå %s ", waytonode1[0] -> data);
                break;

            }    
            i++;
        }

    }
    if (i == 2)
        voice("%s: ", waytonode1[0] -> data);

    PrintForCompair(waytonode1, number1, k, ", ");

    voice("â îòëè÷èè îò %s\n", waytonode2[0] -> data);

    if (k - 1 < number2)
    {
        voice("À %s: ", waytonode2[0] -> data);
        PrintForCompair(waytonode2, number2, k, ", ");
        voice("â îòëè÷èè îò %s", waytonode1[0] -> data);
    }

    printf("\n\n");

    free(waytonode1);
    free(waytonode2);
    return k;
}

