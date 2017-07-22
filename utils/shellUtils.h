#include "../linkedlist/linkedList.h"
#include "../linkedlist/listUtils.h"
#include "../pipes/pipes.h"
#include "myUtils.h"
#include "../process/process.h"
#include "../tokenize/makeArgs.h"
#include "../alias/alias.h"
#include "../history/history.h"

int doCommand(char * s, LinkedList * historyList, LinkedList * aliasList, int histCount);
void aliasSubChecks(char * s, LinkedList * aliasList);
void pathSubChecks(char * s);
void cdSubChecks(char * s);
void bangSubChecks(char * s, LinkedList * historyList, LinkedList * aliasList, int histCount);
char * getCommandFromHistory(char * bangNumber, LinkedList * historyList, int histCount);
int getNumberFromBangCommand(char * s);
void redirectSetup(char * s);
void redirect(char ** command, char * output);
int checkIfAlias(char * s, LinkedList * aliasList);
void pipeCommand(char * s);
void forkCommand(char * s);
void trim(char ** word);
