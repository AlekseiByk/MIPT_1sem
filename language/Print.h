void GraphPrintTree(FILE *dump, tree_node_t* headNode)
{
    assert(dump);

    if (headNode -> data.type == TYPENUMBER)
        fprintf(dump, "\"%p\"[shape = egg, color = \"darkseagreen\", label = \"%lg\"];\n", headNode, headNode -> data.value);

    else if (headNode -> data.type == TYPEVARIABLE)
        fprintf(dump, "\"%p\"[shape = egg, color = \"darkolivegreen1\", label = \"%s\"];\n", headNode, variables[(int)headNode -> data.value].name);

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value < 6))
        fprintf(dump, "\"%p\"[shape = diamond, color = \"gold\", label = \"%s\"];\n", headNode, takefunc((int) headNode -> data.value));

    else if (headNode -> data.type == TYPEFUNCTION)
        fprintf(dump, "\"%p\"[shape = box, color = \"grey\", label = \"%s%s\"];\n", headNode, (headNode -> data.value > 24)? "$" : "", takefunc((int) headNode -> data.value));

    else if (headNode -> data.type == TYPEOPERATOR)
        fprintf(dump, "\"%p\"[shape = %s, color = \"%s\", label = \"%s\"];\n", headNode, operators[(int) headNode -> data.value].form, operators[(int) headNode -> data.value].color, operators[(int) headNode -> data.value].name);

    else
        fprintf(dump, "\"%p\"[shape = parallelogram, color = \"grey56\", label = \"%s\"];\n", headNode, compare_operators[(int) headNode -> data.value].name);

    if(headNode -> left !=  nullptr)
    {
        fprintf(dump, "\"%p\" -> \"%p\";\n", headNode, headNode -> left);

        GraphPrintTree(dump, headNode -> left);
    }

    if(headNode -> right !=  nullptr)
    {
        fprintf(dump, "\"%p\" -> \"%p\";\n", headNode, headNode -> right);

        GraphPrintTree(dump, headNode -> right);
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

    fprintf(dump, "digraph listDump\n{\n\n");

    GraphPrintTree(dump, headNode);
    
    fprintf(dump, "}");
    fclose(dump);

    Systemf("dot -Tpdf  Dump%02d.dot -o Dump%02d.pdf", DumpNumber, DumpNumber);
    //Systemf("rm Dump%02d.dot", DumpNumber);
    //Systemf("evince Dump%02d.pdf", DumpNumber);

    DumpNumber++;    
}

void WriteTree(FILE *Write, tree_node_t* headNode)
{
    assert(headNode);
    assert(Write);

    fprintf(Write, "{");

    if (headNode -> data.type == TYPENUMBER)
        fprintf(Write, "%lg", headNode -> data.value);

    else if (headNode -> data.type == TYPEVARIABLE)
        fprintf(Write, "%s", variables[(int)headNode -> data.value].name);

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value <= 24))
        fprintf(Write, "%s", takefunc((int) headNode -> data.value));

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value > 24))
        fprintf(Write, "$%s", takefunc((int) headNode -> data.value)); 

    else if (headNode -> data.type == TYPEOPERATOR)
        fprintf(Write, "%s", operators[(int) headNode -> data.value].name);   

    else
        fprintf(Write, "%s", compare_operators[(int) headNode -> data.value].name);


    if(headNode -> left !=  nullptr || headNode -> right !=  nullptr)
    {
        if(headNode -> left !=  nullptr)
            WriteTree(Write, headNode -> left);

        else
            fprintf(Write, "@");

        if(headNode -> right !=  nullptr)
            WriteTree(Write, headNode -> right);

        else
            fprintf(Write, "@");
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

tree_node_t* readTree()
{
    tree_node_t* node = nullptr;

    if ((*s) == '@')
    {
        s++;
        return node;
    }

    if ((*s) == '{')
    {
        s++;

        char text[MAXCOMMANDLEN] = "";
        int i = 0;

        sscanf(s, " %[^{}@ ] %n", text, &i);
        s += i;

        if (strcmp(";", text) == 0)
            node = CreateNode(TYPEOPERATOR, SEMICOLONop);

        else if (strcmp(",", text) == 0)
            node = CreateNode(TYPEOPERATOR, COMMAop);

        else if (strcmp("if", text) == 0)
            node = CreateNode(TYPEOPERATOR, IFop);

        else if (strcmp("if-else", text) == 0)
            node = CreateNode(TYPEOPERATOR, IFELSEop);

        else if (strcmp("while", text) == 0)
            node = CreateNode(TYPEOPERATOR, WHILEop);

        else if (strcmp("op", text) == 0)
            node = CreateNode(TYPEOPERATOR, OPop);

        else if (strcmp("ret", text) == 0)
            node = CreateNode(TYPEOPERATOR, RETURNop);

        else if (strcmp("=", text) == 0)
            node = CreateNode(TYPEOPERATOR, ASSIGNop);

        else if (isdigit(text[0]))
            node = CreateNode(TYPENUMBER, atoi(text));

        else if (strcmp("==", text) == 0)
            node = CreateNode(TYPECOMPARE, 0);

        else if (strcmp("!=", text) == 0)
            node = CreateNode(TYPECOMPARE, 1);

        else if (strcmp(">", text) == 0)
            node = CreateNode(TYPECOMPARE, 2);

        else if (strcmp(">=", text) == 0)
            node = CreateNode(TYPECOMPARE, 3);

        else if (strcmp("<", text) == 0)
            node = CreateNode(TYPECOMPARE, 4);

        else if (strcmp("<=", text) == 0)
            node = CreateNode(TYPECOMPARE, 5);

        else if (text[0] == '$')
            node = CreateNode(TYPEFUNCTION, functionlist(text + 1));

        else if ((*s) == '}')
            node = CreateNode(TYPEVARIABLE, findVariable(text));

        else 
            node = CreateNode(TYPEFUNCTION, functionlist(text));

        node -> left = readTree();

        node -> right = readTree();

        if ((*s) == '}')
            s++;
    }

    return node;  
}

tree_node_t* readTreeFrom(char* input)
{
    assert(input);

    s = input;

    return readTree();
}

struct MyCommands_t{
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
                        {"вытворная","deriv"}

};
const int COMMANDSNUMBER = 12;

char* ReplaceCode(char* incode, int num)
{
    for (int i = 0; i < COMMANDSNUMBER; i++)
    {
        if (num == 1)
        {
            if ( strcmp (MyLangCommands[i].MyLang, incode) == 0 )
                return MyLangCommands[i].CLang;
        }

        else
            if ( strcmp (MyLangCommands[i].CLang, incode) == 0 )
                return MyLangCommands[i].MyLang;
    }

    return incode;
}

char* ReCode(char* input, int num)
{
    char* outcode = (char*) calloc(strlen(input), sizeof(outcode[0]));

    s = input;

    while ((*s) != '\0')
    {

        if ( isspace(*s) || ispunct(*s))
        {
            outcode[strlen(outcode)] = *s;

            s++;
        }
        else
        {
            int n = 0;
            char text[MAXCOMMANDLEN] = "";
            sscanf(s, "%[^ \t\n(),=><!/*+^/-]%n", text, &n);

            s += n;

            strcat(outcode, ReplaceCode(text, num));
        }
    }

    return outcode;
}

void WriteTreeLang(FILE *Write, tree_node_t* headNode, int tabs = 0)
{
    assert(headNode);
    assert(Write);

    if (headNode -> data.type == TYPENUMBER)
        fprintf(Write, "%lg", headNode -> data.value);

    else if (headNode -> data.type == TYPEVARIABLE)
        fprintf(Write, "%s", variables[(int)headNode -> data.value].name);

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value < 6))
    {
        WriteTreeLang(Write, headNode -> left);
        fprintf(Write, " %s ", takefunc((int) headNode -> data.value));
        WriteTreeLang(Write, headNode -> right);
    }

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value < 22))
    {
        fprintf(Write, "%s ( ", takefunc((int) headNode -> data.value));
        WriteTreeLang(Write, headNode -> right);
        fprintf(Write, " )");
    }

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value < 24))
    {
        fprintf(Write, "%s ( ", takefunc((int) headNode -> data.value));
        WriteTreeLang(Write, headNode -> left);
        fprintf(Write, " )");
    }

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value == 24))
    {
        fprintf(Write, "%s ( ", takefunc((int) headNode -> data.value));
        WriteTreeLang(Write, headNode -> left);
        fprintf(Write, ", ");
        WriteTreeLang(Write, headNode -> right);
        fprintf(Write, " )");
    }

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value > 24))
    {
        fprintf(Write, "%s( ", takefunc((int) headNode -> data.value));                                                    ///////////////********************
        
        if ((headNode -> left) != nullptr)
            WriteTreeLang(Write, headNode -> left);

        fprintf(Write, " )");
        
        if ((headNode -> right) != nullptr)
            WriteTreeLang(Write, headNode -> right, 1);
    }

    else if (headNode -> data.type == TYPEOPERATOR)
        switch((int) headNode -> data.value)
        {
            case OPop:
                fprintf(Write, "\n");

                for (int i = 0; i < tabs; i++)
                    fprintf(Write, "\t");

                WriteTreeLang(Write, headNode -> left, tabs);

                if ((headNode -> right) != nullptr)
                    WriteTreeLang(Write, headNode -> right, tabs);

                break;

            case SEMICOLONop:
                WriteTreeLang(Write, headNode -> left, 0);

                fprintf(Write, "\n\n");

                if ((headNode -> right) != nullptr)
                    WriteTreeLang(Write, headNode -> right, 0);

                break;

            case COMMAop:
                WriteTreeLang(Write, headNode -> right);

                if ((headNode -> left != nullptr) && (headNode -> left -> data.type == TYPEOPERATOR) && (headNode -> left -> data.value == COMMAop))
                {
                    fprintf(Write, ", ");

                    WriteTreeLang(Write, headNode -> left, 0);
                }
                break;

            case WHILEop:
                fprintf(Write, "while (");                                                                                    ///////////////********************
                
                WriteTreeLang(Write, headNode -> left);
                
                fprintf(Write, " )");
                
                WriteTreeLang(Write, headNode -> right, tabs + 1);
                
                break;

            case IFop:
                fprintf(Write, "if ( ");                                                                                     ///////////////********************
                
                WriteTreeLang(Write, headNode -> left);
                
                fprintf(Write, " )");
                
                WriteTreeLang(Write, headNode -> right, tabs + 1);
                
                break;

            case IFELSEop:
                WriteTreeLang(Write, headNode -> left, tabs);
                
                if  ((headNode -> right) != 0) 
                {
                    fprintf(Write, "\n");
                    
                    for (int i = 0; i < tabs - 1; i++)
                        fprintf(Write, "\t");
                    
                    fprintf(Write, "else");                                                                                    ///////////////********************
                    
                    WriteTreeLang(Write, headNode -> right, tabs);
                }

                break;

            case RETURNop:
                fprintf(Write, "ret "); 
                
                WriteTreeLang(Write, headNode -> left);
                
                fprintf(Write, "\n"); 
                
                break;

            case ASSIGNop:
                WriteTreeLang(Write, headNode -> left);
                
                fprintf(Write, " = ");                                                                                    ///////////////********************
                
                WriteTreeLang(Write, headNode -> right);
                
                break;
    }
    else if (headNode -> data.type == TYPECOMPARE)
    {
        WriteTreeLang(Write, headNode -> left);
        
        fprintf(Write, " %s ", compare_operators[(int) headNode -> data.value].name);                                       ///*//////////**********************
        
        WriteTreeLang(Write, headNode -> right);
    }
}

void WriteTreeToLang(const char *FileOut, tree_node_t* headNode)
{
    assert(headNode);

    FILE* Write = fopen(FileOut, "w");
    assert(Write);

    WriteTreeLang(Write, headNode);
    
    fclose(Write);
}