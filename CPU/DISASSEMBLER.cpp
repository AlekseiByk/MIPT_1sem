#include "headers/Text.h"
#include "headers/Stack.h"
#include "headers/enum.h"

char FileIn[] = "temp/code.bas";
char FileOut[] = "temp/Disassemlered_Code.txt";

///------------------------------------------------------------------------------------------
/// \brief check you code for matching version and coding
/// \param code pointer to executable code
/// \return true if code exist, false if it is not
///------------------------------------------------------------------------------------------

bool CheckCode(const char* code);

void Disassemble(char* code, const int CodeL);



int main() {
    int CodeL = 0;
    char *code = TextFromFile(FileIn, &CodeL);
/*
    for (int i = 0; i < CodeL; i++)
    {
    printf("%d ", code[i]);
    }

    printf("\n");
*/
    if (CheckCode(code))
        Disassemble(code, CodeL);
    else
        printf("code does not exist, please check version and format");

    free(code);
    return 0;
}


bool CheckCode(const char* code)
{
    return code[0] == 'b' && 
           code[1] == 'a' && 
           code[2] == 's' && 

           code[3] == version;
}


void Disassemble(char* code, const int CodeL)
{
  FILE *f = fopen(FileOut, "w");
  int pc = 4;
    while (pc < CodeL)
    {
      
       switch (code[pc++])
       {
           #define DEF_CMD(name, num, ASMcode, CPUcode, DIScode)  \
           case CMD_##name:                                       \
                fputs(#name, f);                                  \
                fputc(' ', f);                                    \
                DIScode;                                          \
                fputc('\n', f);                                   \
                break;

           #include "commands.h"

           #undef DEF_CMD     
       }
    }
    fclose(f);
}
