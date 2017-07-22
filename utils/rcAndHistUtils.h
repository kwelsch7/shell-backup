#include <stdio.h>
#include "shellUtils.h"

void checkMshrcContents(char * mshrcContents, int * histCount, int * histFileCount, char ** setPath);
int getHistValue(char * mshrcContents);
void getPathValue(char * mshrcContents, char ** setPath);
