#include <stdio.h>
//#include <assert.h>
#include <stdlib.h>
#include "Text.h"
#include <string.h>

typedef char element_t;
typedef int Size_t;
typedef unsigned int hash_t;
const int MAXSIZE = 30;
const int SIZEOFTABLE = 1001;

struct string_array_t{
    char name[25];
} commands[] = {{"HashSolve_0"}, {"HashSolve_first_elem"}, {"HashSolve_Len"}, {"HashSolve_Sum"}, {"HashSolve_Rol"}, {"HashSolve_GNU"}};

struct list_elem_t
{
    hash_t hash;
    element_t elem[MAXSIZE] = "";
    list_elem_t* next = NULL;
};

struct hash_table_t
{
    list_elem_t* table;
    int hashSize;
    ///KOTIKI
};

int HashlistAdd(hash_table_t *hashTable, int number, element_t elem[], hash_t hash);
hash_t HashSolve(element_t elem[]);
void WriteTable(hash_table_t hashTable, int funcNumber);
void FillHashTable(char* input, int length, hash_table_t* table, hash_t (*hashfunc)(element_t elem[]));
void HashTableConstruct(hash_table_t* hash, int number);
void HashTableDestruct(hash_table_t* hash);

hash_t HashSolve_0(element_t elem[]);
hash_t HashSolve_first_elem(element_t elem[]);
hash_t HashSolve_Len(element_t elem[]);
hash_t HashSolve_Sum(element_t elem[]);
hash_t HashSolve_Rol(element_t elem[]);
hash_t HashSolve_GNU(element_t elem[]);

int main() 
{
    hash_t (*hashfunc_arr[])(element_t elem[]) = {HashSolve_0, HashSolve_first_elem, HashSolve_Len, HashSolve_Sum, HashSolve_Rol, HashSolve_GNU};
    hash_table_t HashTable = {};
    int length = 0;
    char* input = TextFromFile("input.txt", &length);
    printf("%d\n", length);
    for (int i = 0; i < 6; i++)
    {
        printf("Start>>%d\n", i);
        fflush(0);
        HashTableConstruct(&HashTable, SIZEOFTABLE);
        FillHashTable(input, length, &HashTable, hashfunc_arr[i]);
        WriteTable(HashTable, i);
        HashTableDestruct(&HashTable);
        printf("Done>>%d\n", i);
        fflush(0);
    }


    return 0;
}


int HashlistAdd(hash_table_t *hashTable, element_t elem[], hash_t hash)
{
    assert(hashTable);


    list_elem_t* temp = hashTable -> table + (hash % (hashTable -> hashSize));
    while(temp -> next != NULL)
    {
        if (strcmp(elem, temp -> elem) == 0)
            return 0;
        else
            temp = temp -> next;
    }
    int d = 0;
    while (elem[d] != '\0')
    {
        temp -> elem[d] = elem[d];
        d++;
    }
    temp -> next = (list_elem_t*) calloc(1, sizeof(list_elem_t));
    temp -> hash = hash;
    return 0;
}

void WriteTable(hash_table_t hashTable, int funcNumber)
{
    FILE *f = fopen("results.csv", "a");
    fprintf(f, "%s; ", commands[funcNumber].name); //funcName
    for (int i = 0; i < hashTable.hashSize; i++)
    {
        int k = 0;
        list_elem_t* temp = hashTable.table + i;
        while(temp -> next != NULL)
        {
            //printf("%s ", temp -> elem);
            temp = temp -> next;
            k++;
        }
        fprintf(f, "%d; ", k);
    }
    fprintf(f, "\n");
    fclose(f);
}

void FillHashTable(char* input, int length, hash_table_t* hashTable, hash_t (*hashfunc)(element_t elem[]))
{
    int position = 0;
    while (position < length)
    {
        char word[MAXSIZE] = "";
        int n = 0;
        int scan = sscanf(input + position, "%[a-zA-Z1-9]%n", word, &n);
        if (scan == 1)
        {
            //printf("%s %d\n", word, position);
            hash_t hash = (*hashfunc)(word);
            HashlistAdd(hashTable, word, hash);
            position += n;
        }
        else 
            position++;
    }
}

void HashTableConstruct(hash_table_t* hash, int number)
{
    assert(hash);

    hash -> table = (list_elem_t*) calloc(number, sizeof(list_elem_t));
    assert(hash -> table);

    hash -> hashSize = number;
}

void HashTableDestruct(hash_table_t* hash)
{
    assert(hash);
    assert(hash -> table);
    for (int i = 0; i < hash -> hashSize; i++)
    {
        list_elem_t* temp = &((*hash).table[hash -> hashSize - i - 1]);
        list_elem_t* temp1 = {};
        if (temp != nullptr)
        {
            temp = temp -> next;
            while(temp != nullptr)
            {
                temp1 = temp -> next;
                free(temp);
                temp = temp1;
            }
        }
    }   
    free(hash -> table);
    hash -> hashSize = 0;
}

hash_t HashSolve_0(element_t elem[])
{
    return 0;
}

hash_t HashSolve_first_elem(element_t elem[])
{
    hash_t hash = elem[0];
    return hash;
}

hash_t HashSolve_Len(element_t elem[])
{
    hash_t hash = 0;
    while(true)
        if (elem[hash] == 0)
            return hash;
        else
            hash++; 
    return hash;
}

hash_t HashSolve_Sum(element_t elem[])
{
    hash_t hash = 0;
    int i = 0;
    while(true)
        if (elem[i] == 0)
            return hash;
        else
            hash += elem[i++]; 
    return hash;
}

hash_t HashSolve_Rol(element_t elem[])
{
    hash_t hash = 0;
    int i = 0;
    while(true)
        if (elem[i] == 0)
            return hash;
        else
            hash = /*hash_rol(hash) */1^ elem[i++];; 
    return hash;
}

hash_t HashSolve_GNU(element_t elem[])
{
    hash_t hash = 5381;
    int i = 0;
    while(true)
        if (elem[i] == 0)
            return hash;
        else
            hash = (hash << 5) + hash + elem[i++];; 
    return hash;
}