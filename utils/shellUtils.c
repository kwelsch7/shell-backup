#include "shellUtils.h"

int doCommand(char * s, LinkedList * historyList, LinkedList * aliasList, int histCount)
{
    if(s == NULL)
	{
		perror("s is null");
		exit(-99);
	}
    
    if(strcmp(s, "exit") == 0)
    {
        return -1;
    }

    // Check a bunch of specific commands before passing it off to bash
    if(strcmp(s, "history") == 0)
    {
        printHistoryList(historyList, histCount, stdout);
    }
    else if(strstr(s, "alias") != NULL)
    {
        aliasSubChecks(s, aliasList);
    }
    else if(strstr(s, "PATH") != NULL)
    {
        pathSubChecks(s);
    }
    else if(strstr(s, "cd") != NULL)
    {
        cdSubChecks(s);
    }
    else if(strstr(s, "!") != NULL) // don't forget that these should be pipeable
    {
        bangSubChecks(s, historyList, aliasList, histCount);
    }
    else if(strstr(s, ">") != NULL)
    {
        redirectSetup(s);
    }

    else  // perform command as normal
    {
        int aliasExecuted = checkIfAlias(s, aliasList);
        
        if(!aliasExecuted)  // a normal (or invalid) command
        {
            int pipeCount = containsPipe(s);
            if(pipeCount > 0)
            {
                pipeCommand(s);
            }
            else
            {
                forkCommand(s);
            }
        }
    }

    return 0;
}

void aliasSubChecks(char * s, LinkedList * aliasList)
{
    if(strcmp(s, "alias") == 0) // alias IS the string
    {
        printList(aliasList, printTypeAlias);
    }
    else if(strstr(s, "alias ") == s) // alias at beginning of line
    {
        void * newAlias = buildTypeAlias(s);
        addLast(aliasList, buildNode_Type(newAlias));
    }
    else if(strstr(s, "unalias ") == s) // unalias at beginning of line
    {
        char * copy = (char *) calloc(strlen(s) + 1, sizeof(char));
        char * copyPointer = copy; // for keeping original location of copy after it gets mangled, so it can be freed
        strncpy(copy, s, strlen(s));
        char * unaliasCmd = strtok_r(copy, " ", &copy);
        unaliasCmd = strtok_r(NULL, " ", &copy);

        // Stu says don't worry about "-a"
        void * unaliasAlias = buildTempAlias(unaliasCmd);
        removeItem(aliasList, buildNode_Type(unaliasAlias), cleanTypeAlias, compareAlias);

        free(copyPointer);
        copyPointer = NULL;
    }
    else // alias just in the string
    {
        printf("Invalid command\n");
    }
}

void pathSubChecks(char * s)
{
    if(strcmp(s, "echo $PATH") == 0)
    {
        // May need to consider my local variable
        printf("%s\n", getenv("PATH"));
    }
    else if(strstr(s, "PATH=") == s) // setting path is at beginning of string
    {
        char * copy = (char *) calloc(strlen(s) + 1, sizeof(char));
        char * copyPointer = copy; // for keeping original location of copy after it gets mangled, so it can be freed
        strncpy(copy, s, strlen(s));
        char * newPath = strtok_r(copy, "=", &copy);
        newPath = strtok_r(NULL, "=", &copy);

        if(strstr(newPath, "$PATH") != NULL)
        {
            // assume it's at the beginning of the string and followed by :
            newPath = newPath + 6; // moves the pointer past $PATH:
            char * curPath = getenv("PATH");
            char * fullPath = (char *) calloc(strlen(curPath) + strlen(newPath) + 2, sizeof(char));
            sprintf(fullPath, "%s:%s", curPath, newPath);

            setenv("PATH", fullPath, 1);

            free(fullPath);
            fullPath = NULL;
        }
        else
        {
            setenv("PATH", newPath, 1);
        }

        free(copyPointer);
        copyPointer = NULL;
    }
    else
    {
        printf("Invalid command\n");
    }
}

void cdSubChecks(char * s)
{
    if(strstr(s, "cd ") == s)
    {
        char * copy = (char *) calloc(strlen(s) + 1, sizeof(char));
        char * copyPointer = copy; // for keeping original location of copy after it gets mangled, so it can be freed
        strncpy(copy, s, strlen(s));
        char * newDir = strtok_r(copy, " ", &copy);
        newDir = strtok_r(NULL, " ", &copy);

        int result = chdir(newDir);
        if(result == -1)
        {
            printf("Invalid directory\n");
        }
        else
        {
            char workingDir[128];
            getcwd(workingDir, sizeof(workingDir));
            printf("Directory changed to '%s'\n", workingDir);
        }

        free(copyPointer);
        copyPointer = NULL;
    }
    else
    {
        printf("Invalid command\n");
    }
}

void bangSubChecks(char * s, LinkedList * historyList, LinkedList * aliasList, int histCount)
{
    if(strcmp(s, "!!") == 0)
    {
        // Delete !! as last history item before getting the previous command
        removeLast(historyList, cleanTypeHistory);
        Node * lastCommand = getLast(historyList);
        printf("%s\n", ((History *)(lastCommand->data))->fullLine);
        doCommand(((History *)(lastCommand->data))->fullLine, historyList, aliasList, histCount);
    }
    else if(strstr(s, "|") != NULL)
    {
        char * copy = (char *) calloc(strlen(s) + 1, sizeof(char));
        char * copyPointer = copy; // for keeping original location of copy after it gets mangled, so it can be freed
        strncpy(copy, s, strlen(s));

        // check if I need to trim these
        char * firstCommand = strtok_r(copy, "|", &copy);
        char * secondCommand = strtok_r(NULL, "|", &copy);
        trim(&firstCommand);
        trim(&secondCommand);
        char * actualCmd1 = NULL;
        char * actualCmd2 = NULL;

        if(firstCommand[0] == '!')
        {
            actualCmd1 = getCommandFromHistory(firstCommand, historyList, histCount);
        }
        else
        {
            actualCmd1 = firstCommand;
        }
        if(secondCommand[0] == '!')
        {
            actualCmd2 = getCommandFromHistory(secondCommand, historyList, histCount);
        }
        else
        {
            actualCmd2 = secondCommand;
        }

        char * replacedCommand = (char *) calloc(strlen(actualCmd1) + strlen(actualCmd2) + 4, sizeof(char));
        sprintf(replacedCommand, "%s | %s", actualCmd1, actualCmd2);

        printf("%s\n", replacedCommand);
        doCommand(replacedCommand, historyList, aliasList, histCount);

        free(copyPointer);
        copyPointer = NULL;
        free(replacedCommand);
        replacedCommand = NULL;
    }
    else if(strstr(s, "!") == s) // should be !number; e.g. !25, !2, etc
    {
        // just go replace the command and call doCommand on it
        char * actualCommand = getCommandFromHistory(s, historyList, histCount);
        printf("%s\n", actualCommand);
        doCommand(actualCommand, historyList, aliasList, histCount);
    }
    else
    {
        printf("Invalid command\n");
    }
}

char * getCommandFromHistory(char * bangNumber, LinkedList * historyList, int histCount)
{
    if(historyList == NULL)
    {
        perror("Null list to getCommandFromHistory\n");
        exit(-99);
    }
    if(historyList->size <= 1)  // the 1 because the ! command itself is stored (as far as this assignment goes)
    {
        printf("Nothing in history\n");
    }
    else
    {
        int commandNumber = getNumberFromBangCommand(bangNumber);
        // Do I want the other hist info here???
        if(commandNumber > histCount)
        {
            printf("Command is out of range of saved history\n");
        }
        int curNumber = 1;
        Node * cur = historyList->head->next;

        if(historyList->size > histCount) // and if I do with the other hist info, do I want it here too??
        {
            int startingPoint = historyList->size - histCount;
            while(cur != historyList->tail && curNumber <= startingPoint)
            {
                cur = cur->next;
                curNumber++;
            }
            curNumber = 1;
        }

        while(cur != historyList->tail && commandNumber != curNumber)
        {
            cur = cur->next;
            curNumber++;
        }

        if(cur == historyList->tail) // this shouldn't be reached because of the check of commandNumber vs. histCount earlier
        {
            printf("Unsuccessfully traversed list in getCommandFromHistory");
        }
        else
        {
            return ((History *)(cur->data))->fullLine;
        }
    }
}

int getNumberFromBangCommand(char * s)
{
    char * numberPointer = s + 1;
    int theNumber = atoi(numberPointer);

    return theNumber;
}

void redirectSetup(char * s)
{
    char * copy = (char *) calloc(strlen(s) + 1, sizeof(char));
    char * copyPointer = copy; // for keeping original location of copy after it gets mangled, so it can be freed
    strncpy(copy, s, strlen(s));
    char * from = strtok_r(copy, ">", &copy);
    char * to = strtok_r(NULL, ">", &copy);
    trim(&from);
    trim(&to);
    
    char ** fromArgs = NULL;
    makeargs(from, &fromArgs);

    redirect(fromArgs, to);
}

void redirect(char ** command, char * output)
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
            close(fd[0]);
            
            FILE * outfile = fopen(output, "w");
            char commandOutput[MAX];
            //stdin is the output of the other process, as though it were piped here
            while(fgets(commandOutput, MAX, stdin))
            {
                fprintf(outfile, "%s", commandOutput);
            }

            fclose(outfile);
            exit(0);
        }
        else  // "child" (grandchild of a.out)
        {
            close(fd[0]);
            int thing = close(1);
            int stdout_cpy = dup(fd[1]);
            //dup2(fd[1], 1);
            close(fd[1]);
            execvp(command[0], command);

            // reopen stdout so we don't pipe "Invalid command" to the wc -w and just get 2
            dup2(stdout_cpy, fd[1]);
            dup2(thing, 1);
            printf("Invalid command\n");
            exit(-1);
        }
	}
}

int checkIfAlias(char * s, LinkedList * aliasList)
{
    char * copy = (char *) calloc(strlen(s) + 1, sizeof(char));
    char * copyPointer = copy; // for keeping original location of copy after it gets mangled, so it can be freed
    strncpy(copy, s, strlen(s));
    Alias * checkAlias = buildTempAlias(copy);
    Node * aliasCmd = searchList(aliasList, buildNode_Type(checkAlias), cleanTempAlias, compareAlias);

    int aliasExecuted = 0;
    if(aliasCmd != NULL)
    {
        // Stu says don't worry about the alias containing a pipe
        forkIt(((Alias *)(aliasCmd->data))->argv);
        aliasExecuted = 1;
    }

    free(copyPointer);
    copyPointer = NULL;

    return aliasExecuted;
}

void pipeCommand(char * s)
{
    int preCount = 0, postCount = 0;
    char ** prePipe = parsePrePipe(s, &preCount);
    char ** postPipe = parsePostPipe(s, &postCount);
    pipeIt(prePipe, postPipe);
    clean(preCount, prePipe);
    clean(postCount, postPipe);
}

void forkCommand(char * s)
{
    char **argv = NULL;
    int argc = makeargs(s, &argv);
    if(argc != -1)
    {
        forkIt(argv);
        clean(argc, argv);
        argv = NULL;
    }// end if
}

// remove space ' ' on either end of the string. Does NOT remove it in the middle.
void trim(char ** word)
{
	if(word == NULL)
	{
		perror("word is null");
		exit(-99);
	}// end if

    strip(*word);

	int length = strlen(*word);

    if(length > 1)
    {
        if((*word)[0] == ' ')
        {
            *word = *word + 1;
        }
        if((*word)[length - 1] == ' ')
        {
            (*word)[length - 1] = '\0';
        }
    }
}
