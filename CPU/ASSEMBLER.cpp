#include "headers/Stack.h"
#include <string.h>
#include "headers/enum.h"
#include "headers/Text.h"

char FileIn[] = "temp/input.asmc";
char FileOut[] = "temp/code.bas";
const int MAXLABELES = 100;
const int MAXCOMMAND = 20;

struct label_t{
    char *name;
    int line = -1;
};

struct labelmas_t{
    label_t labelarray[MAXLABELES] = {};
    int count = 0;
};

int Create_Code(char** newline, int strings, labelmas_t* CodeLabeles);
void InputCodeERROR(int line);
void CodeWrite(char* code, char outfile[], int length);
int ScanForRegister(char* line, int* n);
int ScanForNumber(char* line, int* n, int* num);
int dolabel(char* marker, labelmas_t* labelarray, int num);
int ScanForMarker(char* line, int* n, labelmas_t* CodeLabeles);
int takelabel(char* marker, labelmas_t* CodeLabeles);
int ScanForRAM(char* line, int* n, int* num);
void preprocesscode(char* input);
float ScanPUSHPOP(char* line, int* n, float* num);


int main()
{
    char* input = TextFromFile(FileIn);
    
    preprocesscode(input);
    
    printf("\n************************************\n%s\n", input);
    
    int strings = countStr(input) ;
    printf("%d\n", strings);
    char** newline = Create_pointers(input, strings);
	

    labelmas_t CodeLabeles = {};
 
    int error = Create_Code(newline, strings, &CodeLabeles);
    if (error != 0)
        InputCodeERROR(error);

    error = Create_Code(newline, strings, &CodeLabeles);
    if (error == 0)
        printf("\ncompilation completed\n");
    else
        InputCodeERROR(error);

    free (newline);
    free (input);
    return 0;
}

int Create_Code(char** newline, int strings, labelmas_t* CodeLabeles)
{
    char* code = (char*) calloc(strings * (2 * sizeof(char) + sizeof(int)), sizeof(char));
    int pc = 0;


    FILE *log = fopen("temp/log-asm.lst", "wb");
    fprintf(log, "GNU assembler %d.00\nsource: %s\n", (int)version, FileIn);

    for (int i = 0; i < strings; i++) {
        char command[MAXCOMMAND] = "";
        int n = 0, s = 0, n1 = 0, n2 = 0;

        s = sscanf(newline[i], " %n%[:0-9a-zA-Z]%n %n", &n1, command, &n2, &n);
        //printf("%s %d\n", command, s);
        if (s > 0)
        {

            #define DEF_CMD(name, num, ASMcode, CPUcode, DIScode)                                               \
            if (strcmp(#name, command) == 0)                                                                    \
            {                                                                                                   \
                fprintf( log,"%04d  %02X", pc + 4, num);                                                      \
                code[pc++] = num;                                                                               \
                ASMcode;                                                                                        \
                fprintf( log,"%s\n", newline[i]);                                                               \
                if ((sscanf(newline[i] + n, " %s", command) != -1) )                                        	\
                    return i + 1;                                                                           	\
            }                                                                                                   \
            else

            #include "commands.h"
            
            #undef DEF_CMD
            
            if (command[n2 - n1 - 1] == ':')
                dolabel(command, CodeLabeles, pc);
            else 
                return i + 1;  
        }
    }

    CodeWrite(code, FileOut, pc);
    
    fclose (log);

    free (code);
    return 0;
}

void preprocesscode(char* input)
{
	char* comm = strchr(input, ';');
	while (comm != nullptr)
	{
		while (((*comm) != '\n') && ((*comm) != '\0'))
		{
			*comm = ' ';
			comm++;
		}
		comm = strchr(input, ';');
	}
}

void InputCodeERROR(int line)
{
    printf("ERROR: Line: %d\n", line);
    abort();
}

void CodeWrite(char* code, char outfile[], int length)
{
    FILE *f = fopen(outfile, "wb");

    fprintf( f, "bas%c", version);
    fwrite(code, sizeof(char), length, f);

    fclose(f);
}

int ScanForRegister(char* line, int* n)
{
    char command[MAXCOMMAND] = "";
    int n1 = 0, n2 = 0, ntemp = 0;
    int scan = sscanf(line + *n, " %n%[a-dx]%n %n", &n1, command, &n2, &ntemp);
    (*n) += ntemp;
    if (scan == 1) 
    {
        if ((command[1] == 'x') && (n2 - n1) == 2)
        {
            switch (command[0])
            {
                case 'a':
                    return REGISTER_ax;
                case 'b':
                    return REGISTER_bx;
                case 'c':
                    return REGISTER_cx;
                case 'd':
                    return REGISTER_dx;
                default :
                    return -2; 
            }
        }
        return -1;
    }
    return 0;
}

int ScanForNumber(char *line, int* n, float* num)
{
    int scan = sscanf(line, " %*s %f %n", num, n);
    if (scan == 1)
        return NUMBER_VAR;
    return -1;
}

int ScanForMarker(char* line, int* n, labelmas_t* CodeLabeles)
{
    char marker[MAXCOMMAND] = "";
    int n1 = 0;
    int n2 = 0;
    int scan = sscanf(line, " %*s %n%s%n %n", &n1, marker, &n2, n);
    //printf("//%d %s\n", scan, marker);
    if (scan != -1)
        if (marker[n2 - n1 - 1] == ':')
                return takelabel(marker, CodeLabeles);
        else
        {
            float num = -2;
            scan = ScanForNumber(line, n, &num);

            if (scan == -1)
                return -2;
            else 
                return num;
        }
    else
        return -2;
}


int dolabel(char* marker, labelmas_t* CodeLabeles, int num)
{
    for(int i = 0; i < CodeLabeles -> count; i++)
        if (strcmp (CodeLabeles -> labelarray[i].name, marker) == 0)
        {
            CodeLabeles -> labelarray[i].line = num;
            return 0;
        }

    CodeLabeles -> labelarray[ CodeLabeles -> count].name = strdup( marker );
    CodeLabeles -> labelarray[ CodeLabeles -> count].line = num;

    CodeLabeles -> count++;

	return 1;
}

int takelabel(char* marker, labelmas_t* CodeLabeles)
{
    for(int i = 0; i < CodeLabeles -> count; i++)
        if (strcmp (CodeLabeles -> labelarray[i].name, marker) == 0)
            return CodeLabeles -> labelarray[i].line;

    CodeLabeles -> labelarray[ CodeLabeles -> count].name = strdup( marker );
    CodeLabeles -> labelarray[ CodeLabeles -> count].line = -1;

    CodeLabeles -> count++;

    return -1;
}

float ScanPUSHPOP(char* line, int* n, float* num)
{
    int temp = 0, ntemp = 0;
    if ((*(line + *n)) == '[')
    {
    	temp = MEMORY_VAR;
    	(*n)++;
    }

    int scan = sscanf(line + *n, " %f %n", num, &ntemp);
    (*n) += ntemp;
    ntemp = 0;
	if (scan > 0)
		temp += NUMBER_VAR;

    scan = ScanForRegister(line, n);
    if (scan > 0)
    	temp += scan;
    if (scan < 0)
    	return -1;

    if ((*(line + *n)) == '+')
    	(*n)++;
    else if ((temp / MEMORY_VAR) == 1)
    	if ((*(line + *n)) == ']')
    	{
    		(*n)++;
    		return temp;
    	}
    	else
    		return -1;
    else
    	return temp;

    scan = sscanf(line + *n, " %f %n", num, &ntemp);
    (*n) += ntemp;
    ntemp = 0;
	if (scan == 1)
		temp += NUMBER_VAR;	

    scan = ScanForRegister(line, n);
    if (scan > 0)
    	temp += scan;
    if (scan < 0)
    	return -1;

    if ((*(line + *n)) == ']')
	{
		(*n)++;
		return temp;
	}
	else
		return -1;

	return -1;
}
