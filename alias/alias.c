#include "alias.h"

void cleanTypeAlias(void * ptr)
{
    if(ptr == NULL)
    {
        perror("Null parameter to cleanTypeAlias\n");
        exit(-99);
    }

    free(((Alias *)ptr)->command);
    ((Alias *)ptr)->command = NULL;
    clean(((Alias *)ptr)->argc, ((Alias *)ptr)->argv);
    ((Alias *)ptr)->argv = NULL;
}

void cleanTempAlias(void * ptr)
{
    if(ptr == NULL)
    {
        perror("Null parameter to cleanTempAlias\n");
        exit(-99);
    }

    free(((Alias *)ptr)->command);
    ((Alias *)ptr)->command = NULL;
}

void * buildTypeAlias(char * cmd)
{
    if(cmd == NULL)
    {
        perror("Null parameter to buildTypeAlias\n");
        exit(-99);
    }

    char * copy = (char *) calloc(strlen(cmd) + 1, sizeof(char));
	char * copyPointer = copy; // for keeping original location of copy after it gets mangled, so it can be freed
	strncpy(copy, cmd, strlen(cmd));
    char * aliasCmd = strtok_r(copy, " ", &copy);
    aliasCmd = strtok_r(NULL, "=\"", &copy);
    char * actualCmd = strtok_r(NULL, "\"", &copy);

    Alias * newAlias = (Alias *) calloc(1, sizeof(Alias));
    newAlias->command = (char *) calloc(strlen(aliasCmd) + 1, sizeof(char));
    strncpy(newAlias->command, aliasCmd, strlen(aliasCmd));
    int argc = makeargs(actualCmd, &(newAlias->argv));
    newAlias->argc = argc;

    free(copyPointer);
    copyPointer = NULL;

    return newAlias;
}

void * buildTempAlias(char * aliasCmd)
{
    if(aliasCmd == NULL)
    {
        perror("Null parameter to buildTempAlias\n");
        exit(-99);
    }

    Alias * tempAlias = (Alias *) calloc(1, sizeof(Alias));
    tempAlias->command = (char *) calloc(strlen(aliasCmd) + 1, sizeof(char));
    strncpy(tempAlias->command, aliasCmd, strlen(aliasCmd));

    return tempAlias;
}

void printTypeAlias(void * passedIn)
{
    if(passedIn == NULL)
    {
        perror("Null parameter to printTypeAlias\n");
        exit(-99);
    }
    
    // loop through argv and create a string out of it
    char * fullCommand = (char *) calloc(100, sizeof(char));
    char * currentLocation = fullCommand;
    int x;
    for(x = 0; x < ((Alias *)passedIn)->argc; x++)
    {
        int length = strlen((((Alias *)passedIn)->argv)[x]);
        strncpy(currentLocation, (((Alias *)passedIn)->argv)[x], length);
        
        if(((Alias *)passedIn)->argc - 1 != x)
        {
            currentLocation += length;
            *currentLocation = ' ';
            currentLocation++;
        }
    }

    printf("alias %s=\"%s\"\n", ((Alias *)passedIn)->command, fullCommand);

    free(fullCommand);
}

int compareAlias(const void * p1, const void * p2)
{
    if(p1 == NULL || p2 == NULL)
    {
        perror("Null parameter(s) to compareAlias\n");
        exit(-99);
    }

    char * c1 = ((Alias *)p1)->command;
    char * c2 = ((Alias *)p2)->command;
    int curLetter = 0;
    int shorterAliasLength = strlen(((Alias *)p1)->command);
    int secondAliasLength = strlen(((Alias *)p2)->command);
    if(secondAliasLength < shorterAliasLength)
    {
        shorterAliasLength = secondAliasLength;
    }
    
    int result = c1[curLetter] - c2[curLetter];
    curLetter++;
    
    while(result == 0 && curLetter != shorterAliasLength)
    {
        result = c1[curLetter] - c2[curLetter];
        curLetter++;
    }

    return result;
}