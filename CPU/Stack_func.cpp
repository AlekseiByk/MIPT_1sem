//
// Created by Aleksei on 04.10.2019.
//
#include "headers/Stack.h"


#define STK_CREATE_SIZE ((stk -> size) * sizeof(Element_t) + 2 * sizeof(Canary_t))
#define STK_CANARY_PTR (stk -> data + (stk -> size) * sizeof(Element_t) + sizeof(Canary_t))

enum
{
    ///error with pointer *error code
    PTR_ERR = 2,
    ///error dynamic canary 1 pointer *error code
    PTR_DCANARY1_ERR,
    ///error with value array pointer *error code
    PTR_VAL_ERR,
    ///error with dynamic canary 1 pointer *error code
    PTR_DCANARY2_ERR,
    ///error with canaries *error code
    CANARY_ERR,
    ///error with element number *error code
    NUMBER_ERR,
    ///error with stack size *error code
    SIZE_ERR,
    ///error with poison elements *error code
    POISON_ERR,
    ///error with hash *error code
    HASH_ERR,
    ///error if the number is 0 and we want to take the element *error code
    EUNDERFLOW = -1,
    ///Minimum size of array that can be
    MINARRAYSIZE = 5,
    ///coefficient for minimize array (number < size / coeff)
    COEFFTOREALLOC = 3
};

///---------------------------------------------------------------------------------------------------------------------
/// \brief fill mass from FElem to LElem with val
/// \param buff pointer to array
/// \param FElem First element
/// \param LElem Last element
/// \param val input value
///---------------------------------------------------------------------------------------------------------------------

void FillWith(Element_t* buff, int FElem, int LElem, Element_t val)
{
    assert(buff);

    for (int i = FElem; i < LElem; i++)
        buff[i] = val;
}

///---------------------------------------------------------------------------------------------------------------------
/// \brief create pointers and canaries in dynamic array
/// \param stk pointer to stack
///---------------------------------------------------------------------------------------------------------------------

void DoDynamics(Stack_t* stk)
{
    assert( stk );
    assert(stk -> data);

    stk -> values = (Element_t*)(stk -> data + sizeof(Canary_t));
    stk -> popug_dynamic1 = (Canary_t*) stk -> data;
    stk -> popug_dynamic2 = (Canary_t*) STK_CANARY_PTR;
    *(stk -> popug_dynamic1) = CANARY;
    *(stk -> popug_dynamic2) = CANARY;

    FillWith(stk->values, stk -> number, stk->size, POISON);
}

///---------------------------------------------------------------------------------------------------------------------
/// \brief solves hash of stack
/// \param stk pointer to stack
/// \return hash value
///---------------------------------------------------------------------------------------------------------------------

unsigned long SolveHash(const Stack_t *stk)
{
    assert(stk);
    assert(stk -> data);

    char* sth = (char*) stk;

    unsigned int hash = 5381;
    unsigned int i    = 0;

    for (i = 0; i < ((char*)(&(stk -> hash)) - (char*)(stk)); ++sth, ++i)
    {
        hash = ((hash << 5) + hash) + (*sth);
    }

    sth = (char*) stk -> data;
    for (i = 0; i < (stk->size * sizeof(Element_t)); ++sth, ++i)
    {
        hash = ((hash << 5) + hash) + (*sth);
    }

    return hash;
}
///---------------------------------------------------------------------------------------------------------------------
bool StackConstruct(Stack_t *stk)
{
    assert(stk);

    stk -> popug1 = CANARY;
    stk -> popug2 = CANARY;
    stk -> size = MINARRAYSIZE;
    stk -> number = 0;
    stk -> data = (char*) calloc( STK_CREATE_SIZE, sizeof(char*));
    if (!stk -> data)
        {

        return false;
        }
    DoDynamics(stk);
    stk -> error = 0;
    stk -> count_of_poisons = 0;
    stk -> hash = SolveHash(stk);
    AssertMyStack(stk);
    return true;
}


bool StackDestruct(Stack_t *stk)
{
    AssertMyStack(stk);

    stk -> number = 0;
    stk -> size = 0;
    free(stk->data);
    return true;
}


bool StackPush(Stack_t *stk, Element_t val)
{
    AssertMyStack(stk);

    if (val == POISON)
        (*stk).count_of_poisons += 1;
    SetSize(stk);
    stk -> values[stk -> number] = val;
    stk -> number++;
    stk -> hash = SolveHash(stk);

    AssertMyStack(stk);
    return true;
}

Element_t StackPop(Stack_t *stk, int* err)
{
    AssertMyStack(stk);
    //StackDump(stk);
    if (stk->number == 0)
    {
        if (err)
            *err = EUNDERFLOW;
        AssertMyStack(stk);
        return POISON;
    }

    stk->number--;
    Element_t value = stk->values[stk->number];
    if (stk->values[stk->number] == POISON)
        (*stk).count_of_poisons -= 1;
    stk->values[stk->number] = POISON;
    SetSize(stk);
    stk -> hash = SolveHash(stk);

    AssertMyStack(stk);
    return value;
}

bool SetSize(Stack_t *stk)
{
    assert(stk);

    if (stk -> number == stk -> size)
    {
        stk -> size *= 2;

        char* memory = stk -> data;
        memory = (char*) realloc( memory,STK_CREATE_SIZE);
        if (memory)
        {
            stk -> data = memory;
            DoDynamics(stk);

            stk -> hash = SolveHash(stk);
            AssertMyStack(stk);
            return true;
        }
        else
        {
            printf("Dump(%s) from %s (%d) %s\n", "StackOverFlow", __FILE__, __LINE__, __FUNCTION__);
            StackDump(stdin,stk);
            abort();
        }

    }
    else if ((stk -> size > MINARRAYSIZE) && (stk -> number <= (stk -> size) / COEFFTOREALLOC))
    {
        stk -> size /= 2;
        char* memory = stk -> data;
        memory = (char*) realloc( memory,STK_CREATE_SIZE);
        if (memory)
        {
            stk -> data = memory;
            DoDynamics(stk);

            stk -> hash = SolveHash(stk);
            AssertMyStack(stk);
            return true;
        }
        else
            {
            AssertMyStack(stk);
            return false;
            }
    }
    else
        return true;
}

bool CheckPoison(Stack_t* stk)
{
    assert(stk);

    int count = 0;
    for (int i = 0; i < stk -> size; i++)
        if ((*stk).values[i] == POISON)
            count++;

    return (stk->size - stk->number - count + stk->count_of_poisons);

}


bool StackOk(Stack_t *stk)
{
    assert(stk);

    if (stk -> data == nullptr)                                         {stk -> error = PTR_ERR; return false;}
    if (stk -> values == nullptr)                                       {stk -> error = PTR_ERR; return false;}
    if (stk -> popug_dynamic1 == nullptr)                               {stk -> error = PTR_ERR; return false;}
    if (stk -> popug_dynamic2 == nullptr)                               {stk -> error = PTR_ERR; return false;}
    if ((char*)stk -> popug_dynamic1 != stk -> data)                    {stk -> error = PTR_DCANARY1_ERR; return false;}
    if (stk -> values != (Element_t*)(stk -> data + sizeof(Canary_t)))  {stk -> error = PTR_VAL_ERR; return false;}
    if (stk -> popug_dynamic2 != (Canary_t*) STK_CANARY_PTR)            {stk -> error = PTR_DCANARY2_ERR; return false;}
    if ((stk -> popug1) != CANARY)                                      {stk -> error = CANARY_ERR; return false;}
    if ((stk -> popug2) != CANARY)                                      {stk -> error = CANARY_ERR; return false;}
    if (stk -> popug_dynamic1 == stk -> popug_dynamic2)                 {stk -> error = CANARY_ERR; return false;}
    if (*(stk -> popug_dynamic1) != CANARY)                             {stk -> error = CANARY_ERR; return false;}
    if (*(stk -> popug_dynamic2) != CANARY)                             {stk -> error = CANARY_ERR; return false;}
    if (stk -> number < 0)                                              {stk -> error = NUMBER_ERR; return false;}
    if (stk -> number > stk->size)                                      {stk -> error = SIZE_ERR; return false;}
    if (stk -> size < 1)                                                {stk -> error = SIZE_ERR; return false;}
    if (CheckPoison(stk))                                               {stk -> error = POISON_ERR; return false;}
    if (stk -> hash != SolveHash(stk))                                  {stk -> error = HASH_ERR; return false;}

    return true;
}


void StackDump(FILE* DUMP, Stack_t *stk, bool full)
{
    assert(stk);

    if (full)
    {
        fprintf(DUMP, "/----------------------------------------------- ------------------------------------------------------\n");
        fprintf(DUMP, "\tStack_t [%p] (%s)\n",               stk, (stk -> error == 0)? "ok": "ERROR");
        fprintf(DUMP, "\t{\n\t\tpopug1 = 0x%0llX\n",         stk -> popug1);
        fprintf(DUMP, "\t\tnumber = %d\n",                   stk -> number);
        if (stk -> data)
            fprintf(DUMP, "\t\tData [] [%p]\n\t\t{\n",       stk -> data);

        fprintf(DUMP, "\t\t\tpopug_dynamics1 = 0x%0llX\n", *(stk -> popug_dynamic1));
    }
    else
        fprintf(DUMP, "/-----------------------------------------------\n");


    if (stk -> values)
    {
        fprintf(DUMP, "%sValue[%d] [%p]\n%s{\n",                (full)? "\t\t\t" : "",    stk -> size, stk -> values, (full)? "\t\t\t" : "");

    for (int i = 0; i < stk -> size; i++)
        if (i < stk -> number)
            fprintf(DUMP, "%s*[%d] = " ELEM_PRINT "\n",         (full)? "\t\t\t" : "", i, stk -> values[i]);
        else
            fprintf(DUMP, "%s[%d] = " ELEM_PRINT " (POISON)\n", (full)? "\t\t\t" : "", i, stk -> values[i]);
    }


    if (full)
    {
        fprintf(DUMP, "\t\t\t}\n\t\t\tpopug_dynamics2 = 0x%0llX\n\t\t}", *(stk -> popug_dynamic2));
        fprintf(DUMP, "\n\t\tNumber of poison elements = %d",              stk -> count_of_poisons);
        fprintf(DUMP, "\n\t\tError = %d ",                                 stk -> error);


        switch(stk -> error)
        {
            default:
                break;
            case PTR_ERR:
                fprintf(DUMP, "(ERROR with pointers)");
                break;
            case PTR_DCANARY1_ERR:
                fprintf(DUMP, "(ERROR with DynamicCanary1 pointer)");
                break;
            case PTR_DCANARY2_ERR:
                fprintf(DUMP, "(ERROR with DynamicCanary2 pointer)");
                break;
            case PTR_VAL_ERR:
                fprintf(DUMP, "(ERROR with DynamicCanary2 pointer)");
                break;
            case CANARY_ERR:
                fprintf(DUMP, "(ERROR with canaries)");
                break;
            case NUMBER_ERR:
                fprintf(DUMP, "(ERROR with Number)");
                break;
            case SIZE_ERR:
                fprintf(DUMP, "(ERROR with Size)");
                break;
            case POISON_ERR:
                fprintf(DUMP, "(ERROR with Poison elements)");
                break;
            case HASH_ERR:
                fprintf(DUMP, "(ERROR with Hash)");
                break;
        }

        fprintf(DUMP, "\n\t\tHash = %lu\n",        stk -> hash);
        fprintf(DUMP, "\t\tpopug2 = 0x%0llX\n\t}", stk -> popug2);
        fprintf(DUMP, "\n/-----------------------------------------------------------------------------------------------------\n");
    }
    else
        fprintf(DUMP, "\n/-----------------------------------------------\n");
   
}



