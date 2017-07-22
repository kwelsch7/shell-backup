#ifndef HISTORY_H_
#define HISTORY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../linkedlist/linkedList.h"
#include "../utils/myUtils.h"

struct history
{
   char * fullLine;
};
typedef struct history History;

void cleanTypeHistory(void * ptr);
void * buildTypeHistory(char * command);
void printTypeHistory(int count, void * passedIn, FILE * target);
void printHistoryList(const LinkedList * theList, int histCount, FILE * target);

#endif