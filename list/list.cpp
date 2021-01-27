#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


typedef int element_t;
typedef int Size_t;
const int STARTSIZE = 20;


struct list_t
{
	element_t* elements;
	Size_t* next;
	Size_t* prev;
	Size_t head;
	Size_t tail;
	Size_t free;
	Size_t size;
	Size_t maxsize;
};

///-------------------------------------------------------------------------------------------
/// \brief construct list
/// \param list pointer to list
///-------------------------------------------------------------------------------------------

void listConstruct(list_t* list);

///-------------------------------------------------------------------------------------------
/// \brief destruct list
/// \param list pointer to list
///-------------------------------------------------------------------------------------------

void listDestruct(list_t* list);

///-------------------------------------------------------------------------------------------
/// \brief delete element from list
/// \param list pointer to list
/// \param index element index
///-------------------------------------------------------------------------------------------

int listDelete(list_t *list, Size_t index);

///-------------------------------------------------------------------------------------------
/// \brief resise arrays of list if size => maxsize
/// \param list pointer to list
///-------------------------------------------------------------------------------------------

void reSizeIfNeeded(list_t* list);

///-------------------------------------------------------------------------------------------
/// \brief serch for element in the list
/// \param list pointer to list
/// \param element 
/// \return element index, 0 if there isn`t element 
///-------------------------------------------------------------------------------------------

Size_t listSearchFor(list_t *list, Size_t element);

///-------------------------------------------------------------------------------------------
/// \brief serch element by its logic number in the list
/// \param list pointer to list
/// \param number logical number of element
///-------------------------------------------------------------------------------------------

Size_t listFindByLogicNumber(list_t *list, Size_t number);

///-------------------------------------------------------------------------------------------
/// \brief add elementinto the list after index
/// \param list pointer to list
/// \param index element index
/// \param elem pasted element
///-------------------------------------------------------------------------------------------

int listAddAfter(list_t *list, Size_t index, element_t elem);

///-------------------------------------------------------------------------------------------
/// \brief add elementinto the list before index
/// \param list pointer to list
/// \param index element index
/// \param elem pasted element
///-------------------------------------------------------------------------------------------

int listAddBefore(list_t *list, Size_t index, element_t elem);

///-------------------------------------------------------------------------------------------
/// \brief dump list
/// \param list pointer to list
///-------------------------------------------------------------------------------------------

void listDump(list_t* list, bool full = false);

int main() 
{
	list_t list1 = {};
	listConstruct(&list1);
	listAddAfter(&list1, list1.head, 10);
	listDump(&list1);
	listAddAfter(&list1, list1.tail, 20);
	listDump(&list1);
	listAddAfter(&list1, list1.tail, 30);
	listDump(&list1);
	listAddAfter(&list1, list1.tail, 40);
	listDump(&list1);
	listAddAfter(&list1, list1.tail, 50);
	listDump(&list1);
	listAddBefore(&list1, list1.head, 05);
	listDump(&list1);
	return 0;
}

void listConstruct(list_t* list)
{
	assert(list);

	list -> elements = (element_t*) calloc(STARTSIZE, sizeof(element_t));
	list -> next = (Size_t*) calloc(STARTSIZE, sizeof(Size_t));
	list -> prev = (Size_t*) calloc(STARTSIZE, sizeof(Size_t));

	assert(list -> elements);
	assert(list -> next);
	assert(list -> prev);

	for(int i = 1; i < STARTSIZE; i++)
		list -> prev[i] = -1;
	for(int i = 1; i < STARTSIZE - 1; i++)
		list -> next[i] = i + 1;

	list -> head = 0;
	list -> tail = 0;
	list -> free = 1;
	list -> size = 0;
	list -> maxsize = STARTSIZE;
}

void listDestruct(list_t* list)
{
	assert(list);
	assert(list -> elements);
	assert(list -> next);
	assert(list -> prev);

	free(list -> elements);
	free(list -> next);
	free(list -> prev);

	list -> elements = nullptr;
	list -> next = nullptr;
	list -> prev = nullptr;
	list -> head = 0;
	list -> tail = 0;
	list -> free = 0;
	list -> size = 0; 
	list -> maxsize = 0;
}

int listDelete(list_t *list, Size_t index)
{
	assert(list);
	assert(list -> elements);
	assert(list -> next);
	assert(list -> prev);

	if (index != 0)
	{
		reSizeIfNeeded(list);
		list -> tail = (list -> tail == index)? (list -> prev[index]) : list -> tail;
		list -> head = (list -> head == index)? (list -> next[index]) : list -> head;

		list -> next[list -> prev[index]] = list -> next[index];
		list -> prev[list -> next[index]] = list -> prev[index];

		list -> next[index] = list -> free;
		list -> prev[index] = -1;
		list -> free = index;
		list -> size--;
		list -> elements[index] = 0;
		return 0;
	}
	else
		return 1;
}

int listAddAfter(list_t *list, Size_t index, element_t elem)
{
	assert(list);
	assert(list -> elements);
	assert(list -> next);
	assert(list -> prev);
	
	if (list -> head == 0)
	{
		list -> head = list -> free;
		list -> tail = list -> free;

		list -> elements[list -> free] = elem;
		int elem_index = list -> free;
		list -> free = list -> next[list -> free];

		list -> prev[elem_index] = 0;
		list -> next[elem_index] = 0;

		return 0;
	}
	else if (index != 0)
	{
		reSizeIfNeeded(list);
		list -> tail = (list -> tail == index)? (list -> free) : list -> tail;
		list -> head = (list -> head == 0)? 	(list -> free) : list -> head;

		list -> elements[list -> free] = elem;
		int elem_index = list -> free;
		list -> free = list -> next[list -> free];
		list -> prev[list -> next[index]] = (list -> next[index] == 0)? 0 : elem_index;
		list -> prev[elem_index] = index;
		list -> next[elem_index] = (list -> tail == elem_index)? 0 : list -> next[index];
		list -> next[index] = elem_index;

		list -> size++;
		return 0;
	}
	else
		return 1;
}

int listAddBefore(list_t *list, Size_t index, element_t elem)
{
	assert(list);
	assert(list -> elements);
	assert(list -> next);
	assert(list -> prev);
	if (list -> head == 0)
	{
		list -> head = list -> free;
		list -> tail = list -> free;

		list -> elements[list -> free] = elem;
		int elem_index = list -> free;
		list -> free = list -> next[list -> free];

		list -> prev[elem_index] = 0;
		list -> next[elem_index] = 0;

		return 0;
	}
	else if (index != 0)
	{
		reSizeIfNeeded(list);
		list -> head = (list -> head == index)? (list -> free) : list -> head;
		list -> tail = (list -> tail == 0)? 	(list -> free) : list -> tail;

		list -> elements[list -> free] = elem;
		int elem_index = list -> free;
		list -> free = list -> next[list -> free];

		list -> next[elem_index] = index;
		list -> prev[elem_index] = list -> prev[index];
		list -> next[list -> prev[index]] =  (list -> prev[index] == 0)? 0 : elem_index;
		list -> prev[index] = elem_index;

		list -> size++;
		return 0;
	}
	else
		return 1;


}

void reSizeIfNeeded(list_t* list)
{
	assert(list);
	assert(list -> elements);
	assert(list -> next);
	assert(list -> prev);

	if (list -> size == (list -> maxsize - 1))
	{
		int NewSize = list -> maxsize *  2;

		list -> elements =(element_t*) realloc(list -> elements, (NewSize * sizeof(element_t)));
		list -> next =(Size_t*) realloc(list -> elements, (NewSize * sizeof(Size_t)));
		list -> prev =(Size_t*) realloc(list -> elements, (NewSize * sizeof(Size_t)));

		assert(list -> elements);
		assert(list -> next);
		assert(list -> prev);

		for(int i = list -> maxsize; i < NewSize; i++)
			list -> elements[i] = 0;
		for(int i = list -> maxsize; i < NewSize; i++)
			list -> prev[i] = -1;
		for(int i = list -> maxsize; i < NewSize - 1; i++)
			list -> next[i] = i + 1;
		list -> next[NewSize - 1] = 0;

		list -> free = list -> maxsize;
		list -> maxsize = NewSize;
	}
}

Size_t listSearchFor(list_t *list, Size_t element)
{
	for (int i = 1; i <= list -> size; i++)
		if (list -> elements[i] == element)
			return i;
	return 0;
}

Size_t listFindByLogicNumber(list_t *list, Size_t number)
{
	Size_t temp = list -> head;
	for (int i = 0; i < list -> size; i++)
		temp = list -> next[temp];
	return temp;
}

void listDump(list_t* list, bool full)
{
	assert(list);
	assert(list -> elements);
	assert(list -> next);
	assert(list -> prev);

	static unsigned int DumpNumber = 0;

	char filein[] = "Dump00.dot";
	filein[4] = DumpNumber / 10 + 48;
	filein[5] = DumpNumber % 10 + 48;
	FILE* dump = fopen(filein, "wb");

	assert(dump);

	fprintf(dump, "digraph listDump\n{\nnode [shape = record];\n");

	fprintf(dump, "head[label = \"{{%s | %d} | {%s | %d} |{%s | %d} |{%s | %d} |{%s | %d}}\"];\n", "head",list -> head, "tail", list -> tail, "free", list -> free, "size", list -> size, "maxsize", list -> maxsize);
	
	Size_t temp = list -> head;
	for (int i = 0; i < list -> size; i++)
	{
		fprintf(dump, "\"%d\":next -> \"%d\":elem [color = red];\n\"%d\":prev -> \"%d\":elem [color = blue];\n", temp, list -> next[temp], list -> next[temp], temp);
		temp = list -> next[temp];
	}
	/*
		temp = list -> free;
		for (int i = 0; i < list -> maxsize - list -> size - 1; i++)
		{
			fprintf(dump, "\"%d\":next -> \"%d\":elem [color = green];\n", temp, list -> next[temp]);
			temp = list -> next[temp];
		}
	*/
	for (int i = 0; i < list -> maxsize; i++)
		fprintf(dump, "%d[label = \"{ {%d |<elem> %d} | {<next> %d|<prev> %d}}\"];\n", i, i, list -> elements[i], list -> next[i], list -> prev[i]);

	fprintf(dump, "}");
	fclose(dump);

	char topng[] = "dot -Tps  Dump00.dot -o Dump00.ps";
	topng[14] = DumpNumber / 10 + 48;
	topng[15] = DumpNumber % 10 + 48;
	topng[28] = DumpNumber / 10 + 48;
	topng[29] = DumpNumber % 10 + 48;
	system(topng);

	char del[]= "rm Dump00.dot";
	del[7] = DumpNumber / 10 + 48;
	del[8] = DumpNumber % 10 + 48;
	system(del);

	DumpNumber++;

}

