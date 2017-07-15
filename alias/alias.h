#ifndef ALIAS_H_
#define ALIAS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/myUtils.h"
#include "../tokenize/makeArgs.h"

struct alias
{
   char * command;
   char ** argv;
   int argc;
};
typedef struct alias Alias;

void cleanTypeAlias(void * ptr);
void cleanTempAlias(void * ptr);
void * buildTypeAlias(char * command);
void * buildTempAlias(char * aliasCmd);
void printTypeAlias(void * passedIn);
int compareAlias(const void * p1, const void * p2);

#endif