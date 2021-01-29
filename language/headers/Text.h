#ifndef TEXT_H
#define TEXT_H


#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include "ASSert.h"

///-------------------------------------------------------------------------------------
/// \brief 
/// \param Name
/// \param count
/// \return 
///-------------------------------------------------------------------------------------

char* TextFromFile(const char* Name, int* count = nullptr)
{
    struct stat info{};

    FILE *f = fopen(Name, "rb");//open
    assert(f);

    stat(Name, &info);

    char *text = (char *) calloc(info.st_size, sizeof(text[0]));
    int count_ = fread(text, sizeof(text[0]), info.st_size, f);
    assert(count_ > 0);

    if (count)
        *count = count_;

    fclose(f);
    return text;
}

///-------------------------------------------------------------------------------------
/// \brief 
/// \param Name
/// \param count
/// \return 
///-------------------------------------------------------------------------------------

int countStr(char* buff)
{
    assert(buff);

    int sNumber = 0;

    int i = 0;
    while (buff[i] != '\0')
    {
        if (buff[i] == '\n')
        {
            sNumber += 1;
        }
        i++;
    }
    sNumber += 1;

    return sNumber;
}

///-------------------------------------------------------------------------------------
/// \brief 
/// \param Name
/// \param count
/// \return 
///-------------------------------------------------------------------------------------

char** Create_pointers(char *text, int sNumber){
    assert(text != nullptr);
    assert(sNumber > 0);
    char** index = (char**) calloc(sNumber, sizeof(index[0]));
    assert(index != nullptr);

    index[0] = text;

    int i = 0;
    int k = 1;
    while (text[i] != '\0')
    {
        if (text[i] == '\n')
        {
            text[i] = '\0';
            index[k++] = text + i + 1;
        }
        i++;
    }
    return index;
}
    

#endif //TEXT_H
