#ifndef HISTORY_H_
#define HISTORY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/myUtils.h"

struct history
{
   // the history number?
   char * command;
   char ** argv;
   int argc;
};
typedef struct history History;

void cleanTypeHistory(void * ptr);
void * buildTypeHistory(FILE * fin);
void printTypeHistory(void * passedIn);

#endif