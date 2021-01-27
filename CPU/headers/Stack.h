//
// Created by Aleksei on 04.10.2019.
//

#ifndef TEST_STACK_H
#define TEST_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include "ASSert.h"

#define FULLSTACKASERT

#ifndef FULLSTACKASERT
#define AssertMyStack(stk)                                                                                              \
    do {                                                                                                                \
        if (!StackOk(stk), true)                                                                                        \
        {                                                                                                               \
            printf("Dump(%s) from %s (%d) %s\n", "Just test", __FILE__, __LINE__, __FUNCTION__);                        \
            StackDump(stdin,stk, true);                                                                                       \
            exit(1);                                                                                                    \
        }                                                                                                               \
    } while(0)
#else
#define AssertMyStack(stk)      do {} while(0)
#endif


#define ELEM_PRINT "%f"
/// type of Stack elements
typedef float Element_t;
/// poison elements of stack
const int POISON = -136528256;


/// type of protected canaries elements
typedef long long Canary_t;
/// canary const value
const Canary_t CANARY = 0xBEDAB0DADEADBEEF;

struct Stack_t
{
    Canary_t popug1;
    int number;
    char* data;
    Element_t* values;
    Canary_t* popug_dynamic1;
    Canary_t* popug_dynamic2;
    int size;
    int count_of_poisons;
    int error;
    unsigned long hash;
    Canary_t popug2;
};

///---------------------------------------------------------------------------------------------------------------------
/// \brief initialization of stack with pointer stk
/// \param stk pointer to initialize
/// \return true if initialization was successful, false if it was not
///---------------------------------------------------------------------------------------------------------------------

bool StackConstruct(Stack_t *stk);

///---------------------------------------------------------------------------------------------------------------------
/// \brief puts the element to the end of the stack
/// \param stk pointer to the stack
/// \param val value of the element with type Element_t
/// \return true if push was successful, false if it was not
///---------------------------------------------------------------------------------------------------------------------

bool StackPush(Stack_t *stk, Element_t val);

///---------------------------------------------------------------------------------------------------------------------
/// \brief takes the last element of the stack
/// \param stk pointer to the stack
/// \param err pointer to error variable
/// \return value of the last element with type Element_t
///---------------------------------------------------------------------------------------------------------------------

Element_t StackPop(Stack_t *stk, int* err = nullptr);

///---------------------------------------------------------------------------------------------------------------------
/// \brief set new size of stack array
/// \param stk pointer to the stack
/// \return true if changes were successful, false if it was not
///---------------------------------------------------------------------------------------------------------------------

bool SetSize(Stack_t *stk);

///---------------------------------------------------------------------------------------------------------------------
/// \brief delete stack

/// \param stk pointer to the stack
/// \return true if deinitialization was successful, false if it was not
///---------------------------------------------------------------------------------------------------------------------

bool StackDestruct(Stack_t *stk);

///---------------------------------------------------------------------------------------------------------------------

/// \brief print stack info
/// \param stk pointer to the stack
/// \param full if true -> full dump, if false -> not full
///---------------------------------------------------------------------------------------------------------------------

void StackDump(FILE* DUMP = stdin, Stack_t *stk = nullptr, bool full = false);

///---------------------------------------------------------------------------------------------------------------------
/// \brief check stack information
/// \param stk pointer to the stack
/// \return true if stack is okey, false if it is not
///---------------------------------------------------------------------------------------------------------------------

bool StackOk(Stack_t *stk);


#endif //TEST_STACK_H
