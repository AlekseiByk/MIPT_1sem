#include "headers/Text.h"
#include "headers/Stack.h"
#include "headers/enum.h"
#include <math.h>

char FileIn[] = "temp/code.bas";
const int RAMSIZE = 20000;
const int VRAMSIZE = 10000;
char DumpFile[] = "dumps/Dump.txt";

struct CPU_t{
    float ax, bx, cx, dx;
    int pc;
    float* RAM;
    Stack_t stack;
};

///------------------------------------------------------------------------------------------
/// \brief construct of the CPU
/// \param CPU is pointer to CPU
///------------------------------------------------------------------------------------------

void CPUConstruct(CPU_t* CPU,  float* RAM = nullptr);

///------------------------------------------------------------------------------------------
/// \brief check you code for matching version and coding
/// \param code pointer to executable code
/// \return true if code exist, false if it is not
///------------------------------------------------------------------------------------------

bool CheckCode(const char* code);

///------------------------------------------------------------------------------------------
/// \brief do code
/// \param CPU pointer to CPU
/// \param code pointer to code
/// \return 
///------------------------------------------------------------------------------------------

bool DoCode(CPU_t* CPU, char* code);

void CPUDump(FILE* DUMP = stdin, CPU_t* CPU = nullptr);

int main() {
    CPU_t CPU1 = {};
    float *RAM = (float*) calloc(RAMSIZE, sizeof(RAM[0]));
    CPUConstruct (&CPU1, RAM);

    char *code = TextFromFile(FileIn);
/*
    for (int i = 0; i < CodeL; i++)
    {
    printf("%d ", code[i]);
    }

    printf("\n");
*/
    if (CheckCode(code))
        {
        if (!DoCode( &CPU1, code))
                  printf("Error with pointer");
        //CPUDump( &CPU1);
        }
        
    else
        printf("code does not exist, please check version and format. OR I WILL DESTROY YOUR HARD DRIVE!!!    __declspec (muahhahahaha)");

    free(code);
    free(RAM);
    return 0;
}


bool CheckCode(const char* code)
{
    return code[0] == 'b' && 
           code[1] == 'a' && 
           code[2] == 's' && 

           code[3] == version;
}


bool DoCode(CPU_t* CPU, char* code)
{
    while (true)
    {
      FILE* DUMP = fopen(DumpFile, "a");
      CPUDump(DUMP, CPU);
      //printf("%d\n", CPU -> pc);
      fclose(DUMP);
       switch (code[CPU -> pc])
       {
           #define DEF_CMD(name, num, ASMcode, CPUcode, DIScode)   \
           case CMD_##name:                                        \
                CPUcode;                                           \
                break;

           #include "commands.h"

           #undef DEF_CMD     
       }
    }
    return false;
}


void CPUConstruct(CPU_t* CPU, float* RAM)
{
    assert(CPU);
    assert(RAM);

    CPU -> pc = 4;
    CPU -> ax = 0;
    CPU -> bx = 0;
    CPU -> cx = 0;
    CPU -> dx = 0;
    CPU -> RAM = RAM;
 //   CPU -> stack = (Stack_t*) calloc(1, sizeof(Stack_t));
    StackConstruct(&CPU -> stack);
}
void CPUDump(FILE* DUMP, CPU_t* CPU)
{
    assert (CPU);

    fprintf(DUMP, "******************************************************************************************\n");
    fprintf(DUMP, "pc = %d\n", CPU -> pc);
    fprintf(DUMP, "Registers:\n\tax = %f\n", CPU -> ax);
    fprintf(DUMP, "\tbx = %f\n", CPU -> bx);
    fprintf(DUMP, "\tcx = %f\n", CPU -> cx);
    fprintf(DUMP, "\tdx = %f\n", CPU -> dx);
    fprintf(DUMP, "Stack:\n");
    StackDump(DUMP, &CPU -> stack);
    fprintf(DUMP, "RAM(%p):\n", CPU -> RAM);
    //for(int i = 0; i < RAMSIZE - VRAMSIZE; i++)
      //fprintf(DUMP, "%f ", CPU -> RAM[i]);
    fprintf(DUMP, "******************************************************************************************\n");

}


