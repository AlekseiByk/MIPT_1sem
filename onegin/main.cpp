#include <stdio.h>
#include <stdlib.h>
#include <sys\stat.h>
#include <ctype.h>
#include <assert.h>

const char File_in[] =  "Hamlet.txt";
const char File_out[] = "sort.txt";

struct pointer {
    char *adr;
    int len;
};

/// \brief считывает текст из файла в массив возвращая указатель на массив
/// \param Name имя файла
/// \param count указатель на количество символов
/// \return указатель на массив с текстом

char* TextFromFile(const char Name[], int *count);

/// \brief записывает текст в файл через структуру указателей
/// \param index массив указателей
/// \param count количество символов
/// \param f ссылка на FILE

void Filewrite(struct pointer index[], int count, FILE *f);

/// \brief create array of pointers from buffer
/// \param text buffer with text
/// \param cNumber number of chars
/// \param sNumber number of strings
/// \return pointer to pointers array

pointer* Create_pointers(char *text, int cNumber, int *sNumber);

/// \brief swap two elements in array
/// \param v pointer to array
/// \param i first element
/// \param j second element

void swap(struct pointer v[], int i, int j);

/// \brief compare two strings from begining
/// \param a first string
/// \param b second string
/// \return difference between first different elements

int strcmp_begin(const void* arg1, const void* arg2);

/// \brief compare two strings from ending
/// \param a first string
/// \param b second string
/// \return difference between first different elements

int strcmp_end(const void* arg1, const void* arg2);

/// \brief quick sort of array with comp
/// \param v pointer to array
/// \param left the first element of array
/// \param right the last element of array witch will be sorted
/// \param comp

//void qsort(struct pointer v[], int left, int right, int (*comp)(struct pointer, struct pointer));

pointer* arraycopy(pointer* array, int number);

int main() {
    int cNumber = NULL;
    char *buffer = TextFromFile(File_in, &cNumber);

    int strNum = NULL;    ///number of string
    pointer *index = Create_pointers(buffer, cNumber, &strNum);
    pointer *index0 = arraycopy(index, strNum);

    FILE *file = fopen(File_out, "w");

    qsort(index, strNum, sizeof(index[0]), strcmp_begin);

    Filewrite(index, strNum, file);

    qsort(index, strNum, sizeof(index[0]), strcmp_end);

    Filewrite(index, strNum, file);

    Filewrite(index0, strNum, file);

    fclose(file);
    free(index);
    free(buffer);
    free(index0);
    return 0;
}

pointer* arraycopy(pointer* array, int number)
{
    assert(array != nullptr);
    assert(number > 0);

    pointer* copied = (pointer*) calloc(number, sizeof(copied[0]));
    for(int i = 0; i < number; i++)
        copied[i] = array[i];
    return copied;
}

char* TextFromFile(const char Name[], int *count)
{
    struct stat info{};

    FILE *f = fopen(Name, "r");
    assert(f != nullptr);

    stat(Name, &info);

    char *text = (char *) calloc(info.st_size + 1, sizeof(text[0]));
    *count = fread(text, sizeof(text[0]), info.st_size, f);
    assert(*count > 0);

    fclose(f);
    return text;
}

void Filewrite(struct pointer index[], int count, FILE *f)
{
    assert(index != nullptr);
    assert(count > 0);
    assert(f != nullptr);

    fputs("\n***\n\n", f);
    for(int i = 0; i < count; i++)
    {
        fwrite(index[i].adr, sizeof(char), index[i].len, f);
        fputc('\n', f);
    }
}

pointer* Create_pointers(char *text, int cNumber, int *sNumber){
    assert(text != nullptr);
    assert(cNumber != NULL);

    for (int i = 0; i < cNumber; i++)
        if (text[i] == '\n')
            *sNumber += 1;
    *sNumber += 1;


    pointer *index = (struct pointer *) calloc(*sNumber, sizeof(index[0]));
    *sNumber = 0;
    int chars = -1;
    index[0].adr = text;
    for (int i = 0; i <= cNumber; i++) {
        chars++;
        if (text[i] == '\n') {
            text[i] = '\0';
            index[*sNumber + 1].adr = text + i + 1;
            index[*sNumber    ].len = chars;
            chars = -1;
            *sNumber += 1;
        }
    }
    index[*sNumber].len = chars;
    *sNumber += 1;
    assert(index != nullptr);

    return index;
}

void swap(struct pointer v[], int i, int j) {
    struct pointer temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}
/*
void qsort(struct pointer v[], int left, int right, int (*comp)(struct pointer, struct pointer))
{
    if (left >= right)
        return;
    swap(v, left, (left + right)/2);
    int last = left;
    for (int i = left + 1; i <= right; i++)
        if (comp(v[i], v[left]) > 0)
            swap(v, ++last, i);
    swap(v, left, last);

    qsort(v, left, last - 1, comp);
    qsort(v, last + 1, right, comp);
}
*/
int strcmp_begin(const void* arg1, const void* arg2) {
    assert(arg1 != nullptr);
    assert(arg2 != nullptr);

    const pointer a = *((pointer*) arg1);
    const pointer b = *((pointer*) arg2);
    int i = 0, j = 0;
    while ((i < a.len) && (j < b.len)) {
        char c_a = *(a.adr + i);
        char c_b = *(b.adr + j);
        //printf("%c ", c_a);
        //printf("%c\n", c_b);
        if (!isalpha(c_a)){         ///(((c_a >= 'a') && (c_a <= 'z')) || ((c_a >= 'A') && (c_a <= 'Z')))
            i++;
            continue;
        }
        if (!isalpha(c_b)) {
            j++;
            continue;
        }
        if (toupper(c_a) != toupper(c_b))
            return (toupper(c_a) - toupper(c_b));
        i++;
        j++;
    }
    return (a.len - b.len);
}

int strcmp_end(const void* arg1, const void* arg2) {
    assert(arg1 != nullptr);
    assert(arg2 != nullptr);

    const pointer a = *((pointer*) arg1);
    const pointer b = *((pointer*) arg2);
    int i = a.len, j = b.len;
    while ((i >= 0) && (j >= 0)) {
        char c_a = *(a.adr + i);
        char c_b = *(b.adr + j);
        //printf("%c ", c_a);
        //printf("%c\n", c_b);
        if (!isalpha(c_a)){
            i--;
            continue;
        }
        if (!isalpha(c_b)) {
            j--;
            continue;
        }
        if (toupper(c_a) != toupper(c_b))
            return (toupper(c_a) - toupper(c_b));
        i--;
        j--;
    }
    return (a.len - b.len);
}