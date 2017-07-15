#include "pipes.h"

int containsPipe(char *s)
{
    int count = 0;
    int length = strlen(s);
    int x;
    for(x = 0; x < length; x++)
    {
        if(s[x] == '|')
        {
            count++;
        }
    }

    return count;
}

char ** parsePrePipe(char *s, int * preCount)
{
    char * copy = (char *) calloc(strlen(s) + 1, sizeof(char));
	char * copyPointer = copy; // for keeping original location of copy after it gets mangled, so it can be freed
	strncpy(copy, s, strlen(s));
    char * firstCommand = strtok_r(copy, "|", &copy);

    char ** argv = NULL;
    int argCount = makeargs(firstCommand, &argv);

    free(copyPointer);
    copyPointer = NULL;

    *preCount = argCount;
    return argv;
}

char ** parsePostPipe(char *s, int * postCount)
{
    char * copy = (char *) calloc(strlen(s) + 1, sizeof(char));
	char * copyPointer = copy; // for keeping original location of copy after it gets mangled, so it can be freed
	strncpy(copy, s, strlen(s));
    strtok_r(copy, "|", &copy);
    char * secondCommand = strtok_r(NULL, "|", &copy);

    char ** argv = NULL;
    int argCount = makeargs(secondCommand, &argv);

    free(copyPointer);
    copyPointer = NULL;

    *postCount = argCount;
    return argv;
}

void pipeIt(char ** prePipe, char ** postPipe)
{
    pid_t pid1, pid2;
    int status;

    pid1 = fork();

	if(pid1 != 0)  // parent
	{
		waitpid(pid1, &status, 0);
	}
	else  // child
	{
		int fd[2];
        int res = pipe(fd);

        if(res < 0)
        {
            printf("Pipe Failure\n");
            exit(-1);
        }// end if
        
        pid2 = fork();

        if(pid2 != 0)  // "parent" (child of a.out)
        {
            close(fd[1]);
            close(0);
            dup(fd[0]);
            //dup2(fd[0], 0);
            close(fd[0]);
            execvp(postPipe[0], postPipe);

            printf("Invalid command\n");
            exit(-1);
        }
        else  // "child" (grandchild of a.out)
        {
            close(fd[0]);
            int thing = close(1);
            int stdout_cpy = dup(fd[1]);
            //dup2(fd[1], 1);
            close(fd[1]);
            execvp(prePipe[0], prePipe);

            // reopen stdout so we don't pipe "Invalid command" to the wc -w and just get 2
            dup2(stdout_cpy, fd[1]);
            dup2(thing, 1);
            printf("Invalid command\n");
            exit(-1);
        }
	}
}