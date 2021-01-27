//
// Created by Aleksei on 12.10.2019.
//

#define STACKPUSH(arg)                              \
    StackPush(&CPU -> stack, (arg))

#define STACKPOP                                    \
    StackPop(&CPU -> stack)

#define DEFINE_POP_REG( reg )                       \
    case REGISTER_##reg:                            \
        CPU -> reg = STACKPOP;                      \
        \
        break;
        //printf("pop ->" #reg " = %d\n", CPU -> reg);

#define DEFINE_READ_REG( reg )                      \
    case REGISTER_##reg:                            \
        STACKPUSH(CPU -> reg);                      \
        \
        break;
        //printf("push " #reg " = %d\n", CPU -> reg);
#define JMPREAD                                                                                                                                                                             \
        int scanned = ScanForMarker( newline[i], &n, CodeLabeles);                                                                                                                          \
    if (scanned > -2)                                                                                                                                                                       \
    {                                                                                                                                                                                       \
            fprintf( log,"    %02X %02X %02X %02X  ", *((unsigned char*)&scanned), *((unsigned char*)&scanned + 1), *((unsigned char*)&scanned + 2), *((unsigned char*)&scanned + 3));      \
            *((int *)(code + pc)) = (int) (scanned);                                                                                                                                        \
            pc += sizeof(int);                                                                                                                                                              \
    }                                                                                                                                                                                       \
    else                                                                                                                                                                                    \
        return i + 1;

#define JMPCPU                                                                                      \
    if ((int) *((int*)(code + CPU -> pc)) == -1)                                                    \
        return false;                                                                               \
    CPU -> pc = (int) *((int*)(code + CPU -> pc)) + 4;                                              
    //printf("jmp*%d*\n", CPU -> pc);

#define LOG_FPRINT_NOARG fprintf( log,"%17c", ' ')

#define DISPRINTNUMBER fprintf(f, " %d", *((int*)(code + pc)));                        \
                       pc += sizeof(int);

#define DISPRINTREG(byte) switch(byte)                      \
                       {                                    \
                        case 00:                            \
                        break;                              \
                        case 11:                            \
                        fputs("ax", f);                     \
                        break;                              \
                        case 12:                            \
                        fputs("bx", f);                     \
                        break;                              \
                        case 13:                            \
                        fputs("cx", f);                     \
                        break;                              \
                        case 14:                            \
                        fputs("dx", f);                     \
                        break;                              \
                       }                                    \
                       pc++;


DEF_CMD(push, 11,
{
    float num = 0;
    int scanned = ScanPUSHPOP( newline[i], &n, &num); 
    //printf("push %d\n", scanned); fflush(0); 
    if (scanned > 0)                                                                                                                                                                       \
    {    
        *(code + pc) = scanned;
        pc++;
        fprintf( log," %02X", scanned);
        if (((scanned % MEMORY_VAR) / NUMBER_VAR) == 1)
        {
            fprintf( log," %02X %02X %02X %02X  ", *((unsigned char*)&num), *((unsigned char*)&num + 1), *((unsigned char*)&num + 2), *((unsigned char*)&num + 3));
            *((float *)(code + pc)) = num;
            pc += sizeof(float);
        }
        else
            fprintf( log,"              ");
    }  
    else
        return i + 1;    
},

{
    CPU -> pc++;
    if ( (code[CPU -> pc] / MEMORY_VAR) != 1)
    {
        if ( code[CPU -> pc] / NUMBER_VAR == 1)
        {
            CPU -> pc++;
            //printf("%d:push %d\n", CPU -> pc,(int) *((int*)(code + CPU -> pc)));
            STACKPUSH((float) *((float*)(code + CPU -> pc)));
            CPU -> pc += sizeof(float);
        }
        else
        {
            switch(code[CPU -> pc])
            {
                DEFINE_READ_REG( ax );
                DEFINE_READ_REG( bx );
                DEFINE_READ_REG( cx );
                DEFINE_READ_REG( dx );
            }
            CPU -> pc++;
        }
    }
    else
    {

        int TemporaryCode = code[CPU -> pc] - MEMORY_VAR;
        int temp = 0;
        if ((TemporaryCode % NUMBER_VAR) > 0)
        {
            switch(TemporaryCode % NUMBER_VAR)
            {
                DEFINE_READ_REG( ax );
                DEFINE_READ_REG( bx );
                DEFINE_READ_REG( cx );
                DEFINE_READ_REG( dx );
                default:
                    break;
            }
            temp = 2;
        }   
        if ( TemporaryCode / NUMBER_VAR == 1)
        {
            CPU -> pc++;
            //printf("%d:push %d\n", CPU -> pc,(int) *((int*)(code + CPU -> pc)));
            STACKPUSH((float) *((float*)(code + CPU -> pc)));
            CPU -> pc += sizeof(float);
            temp++;
        }
        if (temp == 1)
            STACKPUSH(CPU -> RAM[(int)STACKPOP]);
        if (temp == 2)
        {
            CPU -> pc++;
            STACKPUSH(CPU -> RAM[(int)STACKPOP]);
        }
        if (temp == 3)
        {
            STACKPUSH(CPU -> RAM[(int)STACKPOP + (int)STACKPOP]);
        }

    }
}
,

{
    /*if (code[pc++] / NUMBER_VAR == 1)
        {DISPRINTNUMBER;}
    else
        {DISPRINTREG(code[pc++]);}*/
    if ( (code[pc] / MEMORY_VAR) != 1)
    {
        if ( code[pc] / NUMBER_VAR == 1)
        {
            fprintf(f, " %f", *((float*)(code + pc + 1)));
            pc += sizeof(int) + 1; 
        }
        else
           { DISPRINTREG(code[pc]);}
    }
    else
    {
        int TemporaryCode = code[pc] - MEMORY_VAR;
        fputs("[", f);
        if ((TemporaryCode % NUMBER_VAR) > 0)
        {
            DISPRINTREG(TemporaryCode % NUMBER_VAR);

            if ( TemporaryCode / NUMBER_VAR == 1)
                fputs("+", f);
        }  
        if ( TemporaryCode / NUMBER_VAR == 1)
        {
            pc++;
            DISPRINTNUMBER;
        }
        fputs("]", f);

    }
})
///************************************************************************************************************************
DEF_CMD(pop, 12,
{
    float num = 0;
    int scanned = ScanPUSHPOP( newline[i], &n, &num);
    //printf("pop %d\n", scanned); fflush(0);
    if (scanned > 0)                                                                                                                                                                       \
    {    
        *(code + pc) = scanned;
        pc++;
        fprintf( log," %02X", scanned);
        if (((scanned % MEMORY_VAR) / NUMBER_VAR) == 1)
        {
            fprintf( log," %02X %02X %02X %02X  ", *((unsigned char*)&num), *((unsigned char*)&num + 1), *((unsigned char*)&num + 2), *((unsigned char*)&num + 3));
            *((float *)(code + pc)) = num;
            pc += sizeof(float);
        }
        else
            fprintf( log,"              ");
    }  
    else
        return i + 1;
},

{
    CPU -> pc++;
    if ( (code[CPU -> pc] / MEMORY_VAR) != 1)
    {
        switch(code[CPU -> pc])
        {
            DEFINE_POP_REG(ax);
            DEFINE_POP_REG(bx);
            DEFINE_POP_REG(cx);
            DEFINE_POP_REG(dx);
            case 0:
                STACKPOP;
                break;
        }
        CPU -> pc++;
    }
    else
    {
        //printf("RAM\n");
        //fflush(0);
        int TemporaryCode = code[CPU -> pc] - MEMORY_VAR;
        int temp = 0;
        if ((TemporaryCode % NUMBER_VAR) > 0)
        {
            switch(TemporaryCode % NUMBER_VAR)
            {
                DEFINE_READ_REG( ax );
                DEFINE_READ_REG( bx );
                DEFINE_READ_REG( cx );
                DEFINE_READ_REG( dx );
                default:
                    break;
            }
            temp = 2;
        }   
        if ( TemporaryCode / NUMBER_VAR == 1)
        {
            CPU -> pc++;
            //printf("%d:push %d\n", CPU -> pc,(int) *((int*)(code + CPU -> pc)));
            STACKPUSH((float) *((float*)(code + CPU -> pc)));
            CPU -> pc += sizeof(float);
            temp++;
        }
        if (temp == 1)
            CPU -> RAM[(int)STACKPOP] = STACKPOP;
        if (temp == 2)
        {
            CPU -> pc++;
            CPU -> RAM[(int)STACKPOP] = STACKPOP;
        }
        if (temp == 3)
        {
            CPU -> RAM[(int)STACKPOP + (int)STACKPOP] = STACKPOP;
        }

    }
}
    
,{
    if ( (code[pc] / MEMORY_VAR) != 1)
    {
            DISPRINTREG(code[pc]);
    }
    else
    {
        int TemporaryCode = code[pc] - MEMORY_VAR;
        fputs("[", f);
        if ((TemporaryCode % NUMBER_VAR) > 0)
        {
            DISPRINTREG(TemporaryCode % NUMBER_VAR);

            if ( TemporaryCode / NUMBER_VAR == 1)
                fputs("+", f);
        }  
        if ( TemporaryCode / NUMBER_VAR == 1)
        {
            pc++;
            DISPRINTNUMBER;
        }
        fputs("]", f);
    }
})

DEF_CMD(in, 15, {LOG_FPRINT_NOARG;},
    {
	CPU -> pc++;
        printf("Input:");
        float a = 0;
        scanf("%f", &a);
        STACKPUSH(a);
    },{})






DEF_CMD(add, 20, {LOG_FPRINT_NOARG;},
    {
	CPU -> pc++;
        //printf("add\n");
        STACKPUSH( STACKPOP + STACKPOP);
    },{})

DEF_CMD(sub, 21, {LOG_FPRINT_NOARG;},
    {
	CPU -> pc++;
        //printf("sub\n");
        float a = STACKPOP;
        float b = STACKPOP;
        STACKPUSH(b - a);
    },{})

DEF_CMD(div, 22, {LOG_FPRINT_NOARG;},
    {
	CPU -> pc++;
        //printf("div\n");
        float a = STACKPOP;
        float b = STACKPOP;
        STACKPUSH( b / a);
    },{})

DEF_CMD(mul, 23, {LOG_FPRINT_NOARG;},
    {
	CPU -> pc++;
        //printf("mul\n");
        STACKPUSH(  STACKPOP * STACKPOP);
    },{})

DEF_CMD(sqrt, 24, {LOG_FPRINT_NOARG;},
    {
	CPU -> pc++;
        //printf("sqrt\n");
        STACKPUSH( sqrt( (float)STACKPOP));
    },{})

DEF_CMD(pow, 25, {LOG_FPRINT_NOARG;},
    {
	CPU -> pc++;
        //printf("mul\n");
	float a = STACKPOP;
	float b = STACKPOP;
	b = pow(b,a);
        STACKPUSH( b );
    },{})







DEF_CMD(out, 01, {LOG_FPRINT_NOARG;},
    {
	CPU -> pc++;
        printf("Out: %lg\n", STACKPOP);
    },{})

DEF_CMD(end, 00, {LOG_FPRINT_NOARG;},
    {
	CPU -> pc++;
        printf("program ended\n");
        return true;
    },{})






DEF_CMD(jmp, 50, {JMPREAD;},
{
	CPU -> pc++;
    JMPCPU;
},{DISPRINTNUMBER;})

DEF_CMD(je, 51, {JMPREAD;},
{
	CPU -> pc++;
    if(STACKPOP == STACKPOP)
        {JMPCPU;}
    else 
        CPU -> pc += sizeof(int);
},{DISPRINTNUMBER;})

DEF_CMD(ja, 52, {JMPREAD;},
{
	CPU -> pc++;
    if(STACKPOP < STACKPOP)
        {JMPCPU;}
    else 
        CPU -> pc += sizeof(int);
},{DISPRINTNUMBER;})

DEF_CMD(jae, 53, {JMPREAD;},
{
	CPU -> pc++;
    if(STACKPOP <= STACKPOP)
        {JMPCPU;}
    else 
        CPU -> pc += sizeof(int);
},{DISPRINTNUMBER;})

DEF_CMD(jb, 54, {JMPREAD;},
{
	CPU -> pc++;
    if(STACKPOP > STACKPOP)
        {JMPCPU;}
    else 
        CPU -> pc += sizeof(int);
},{DISPRINTNUMBER;})

DEF_CMD(jbe, 55, {JMPREAD;},
{
	CPU -> pc++;
    if(STACKPOP >= STACKPOP)
        {JMPCPU;}
    else 
        CPU -> pc += sizeof(int);
},{DISPRINTNUMBER;})

DEF_CMD(jne, 56, {JMPREAD;},
{
	CPU -> pc++;
    if(STACKPOP != STACKPOP)
        {JMPCPU;}
    else 
        CPU -> pc += sizeof(int);
},{DISPRINTNUMBER;})

DEF_CMD(call, 57, {JMPREAD;},
{
	CPU -> pc++;
    STACKPUSH(CPU -> pc + sizeof(int));
    JMPCPU;
},{DISPRINTNUMBER;})

DEF_CMD(ret, 58, {LOG_FPRINT_NOARG;},
{
	CPU -> pc++;
    CPU -> pc = STACKPOP;
},{})

#undef STACKPUSH
#undef STACKPOP
#undef DEFINE_POP_REG
#undef DEFINE_READ_REG
#undef JMPREAD
#undef LOG_FPRINT_NOARG
#undef DISPRINTNUMBER
