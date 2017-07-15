#include "makeArgs.h"

void clean(int argc, char **argv)
{
	int x;
	for(x = 0; x < argc; x++) // the last one (the == to argc in x <=) is for the NULL extra row??
	{
		free(argv[x]);
		argv[x] = NULL;
	}
	free(argv);
}// end clean

void printargs(int argc, char **argv)
{
	int x;
	for(x = 0; x < argc; x++)
		printf("%s\n", argv[x]);

}// end printargs

int makeargs(char *s, char *** argv)
{
	char * delim = " \n\t";
	char * copy = (char *) calloc(strlen(s) + 1, sizeof(char));
	char * copyPointer = copy; // for keeping original location of copy after it gets mangled, so it can be freed
	strncpy(copy, s, strlen(s));

	int tokenCount = 0;
	
	char * dummyVal = strtok_r(copy, delim, &copy);
	if(dummyVal != NULL)
	{
		tokenCount++;
	}
	while(dummyVal = strtok_r(NULL, delim, &copy))
	{
		tokenCount++;
	}

	free(copyPointer);
	copyPointer = NULL;
	if(tokenCount == 0)
	{
		return -1;
	}

	(*argv) = (char **) calloc(tokenCount + 1, sizeof(char *));
	char * token = strtok_r(s, delim, &s);
	strip(token);
	(*argv)[0] = (char *) calloc(strlen(token) + 1, sizeof(char));
	strncpy((*argv)[0], token, strlen(token));
	
	int x;
	for(x = 1; x < tokenCount; x++)
	{
		token = strtok_r(NULL, delim, &s);
		strip(token);
		(*argv)[x] = (char *) calloc(strlen(token) + 1, sizeof(char));
		strncpy((*argv)[x], token, strlen(token));
	}

	// One more to add the NULL terminating token
	//(*argv)[x] = (char *) calloc(1, sizeof(char));
	(*argv)[x] = NULL;

	return tokenCount;
}// end makeArgs
