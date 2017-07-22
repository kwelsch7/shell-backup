#include "rcAndHistUtils.h"

void checkMshrcContents(char * mshrcContents, int * histCount, int * histFileCount, char ** setPath)
{
    if(strstr(mshrcContents, "HISTCOUNT") == mshrcContents)
    {
        *histCount = getHistValue(mshrcContents);
    }
    else if(strstr(mshrcContents, "HISTFILECOUNT") == mshrcContents)
    {
        *histFileCount = getHistValue(mshrcContents);
    }
    else if(strstr(mshrcContents, "alias") == mshrcContents)
    {
        printf("- Reading alias from mshrc not implemented -\n");
    }
    else if(strstr(mshrcContents, "PATH") == mshrcContents)
    {
        getPathValue(mshrcContents, setPath);
    }
    else
    {
        printf("%s\n", mshrcContents); // for testing; I want to check if this is hit on empty lines
        printf("Improper format of .mshrc\n");
    }
}

int getHistValue(char * mshrcContents)
{
    char * copy = (char *) calloc(strlen(mshrcContents) + 1, sizeof(char));
    char * copyPointer = copy; // for keeping original location of copy after it gets mangled, so it can be freed
    strncpy(copy, mshrcContents, strlen(mshrcContents));

    char * histValue = strtok_r(copy, "=", &copy);
    histValue = strtok_r(NULL, "=", &copy);
    trim(&histValue);

    int count = atoi(histValue);

    free(copyPointer);
    copyPointer = NULL;

    return count;
}

void getPathValue(char * mshrcContents, char ** setPath)
{
    char * copy = (char *) calloc(strlen(mshrcContents) + 1, sizeof(char));
    char * copyPointer = copy; // for keeping original location of copy after it gets mangled, so it can be freed
    strncpy(copy, mshrcContents, strlen(mshrcContents));

    char * pathValue = strtok_r(copy, "=", &copy);
    pathValue = strtok_r(NULL, "=", &copy);
    trim(&pathValue);

    *setPath = (char *) calloc(strlen(pathValue) + 1, sizeof(char));
    strncpy(*setPath, pathValue, strlen(pathValue));

    free(copyPointer);
    copyPointer = NULL;
}