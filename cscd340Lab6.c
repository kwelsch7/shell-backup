#include "./linkedlist/linkedList.h"
#include "./linkedlist/listUtils.h"
#include "./pipes/pipes.h"
#include "./utils/myUtils.h"
#include "./process/process.h"
#include "./tokenize/makeArgs.h"
#include "./alias/alias.h"
#include "./history/history.h"

void redirect(char ** command, char * output);
void trim(char ** word);

int main()
{
    // check .mshrc -- create it if necessary w/ default values
    // create/fill history linkedList
    // create/fill alias linkedList
    LinkedList * historyList = linkedList();
    LinkedList * aliasList = linkedList();
    FILE * mshrc = fopen(".mshrc", "r+");
    FILE * msh_history = fopen(".msh_history", "w+");
    int histCount = 100, histFileCount = 1000;
    
    if(mshrc == NULL)
    {
        mshrc = fopen(".mshrc", "w+");
        fprintf(mshrc, "%s%d\n", "HISTCOUNT=", histCount);
        fprintf(mshrc, "%s%d\n", "HISTFILECOUNT=", histFileCount);
        
        // set up alias structure
        // set up PATH structure
    }
    else
    {
        // check format of existing .mshrc
        // fill historyList according to .msh_history contents
        // fill aliasList according to .mshrc contents (if there are any in there)
    }

    // display prompt
    // receive and execute commands
    int argc, pipeCount;
    char **argv = NULL, s[MAX];
    int preCount = 0, postCount = 0;
    char ** prePipe = NULL, ** postPipe = NULL;

    printf("command?: ");
    fgets(s, MAX, stdin);
    strip(s);
    addLast(historyList, buildNode_Type(buildTypeHistory(s)));
    //need to start removing first if after histFileCount!!!!!!!!

    while(strcmp(s, "exit") != 0)
    {
        if(strcmp(s, "history") == 0)
        {
            printHistoryList(historyList, histCount);
        }
        else if(strstr(s, "alias") != NULL)
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
        else if(strstr(s, "PATH") != NULL)
        {
            if(strcmp(s, "echo $PATH") == 0)
            {

            }
            else if(strstr(s, "PATH=") == s) // setting path is at beginning of string
            {

            }
            else
            {

            }
        }
        else if(strstr(s, "cd") != NULL)
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
            }
            else
            {
                printf("Invalid command\n");
            }
        }
        else if(strstr(s, "!") != NULL) // don't forget that these should be pipeable
        {
            if(strcmp(s, "!!") == 0)
            {

            }
            else if(strstr(s, "!") == s)// should be !number; e.g. !25, !2, etc
            {

            }
            else
            {

            }
        }
        else if(strstr(s, ">") != NULL)
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

        else  // perform command as normal
        {
            // check if in alias list
            char * copy = (char *) calloc(strlen(s) + 1, sizeof(char));
            char * copyPointer = copy; // for keeping original location of copy after it gets mangled, so it can be freed
            strncpy(copy, s, strlen(s));
            Alias * checkAlias = buildTempAlias(copy);
            
            Node * aliasCmd = searchList(aliasList, buildNode_Type(checkAlias), cleanTempAlias, compareAlias);
            if(aliasCmd != NULL)
            {
                // Stu says don't worry about the alias containing a pipe
                forkIt(((Alias *)(aliasCmd->data))->argv);
            }

            else  // a normal (or invalid) command
            {
                pipeCount = containsPipe(s);
                if(pipeCount > 0)
                {
                    prePipe = parsePrePipe(s, &preCount);
                    postPipe = parsePostPipe(s, &postCount);
                    pipeIt(prePipe, postPipe);
                    clean(preCount, prePipe);
                    clean(postCount, postPipe);
                }// end if pipeCount

                else
                {
                    argc = makeargs(s, &argv);
                    if(argc != -1)
                    {
                        forkIt(argv);
                        clean(argc, argv);
                        argv = NULL;
                    }// end if
                }
            }

            free(copyPointer);
            copyPointer = NULL;
        }

        printf("command?: ");
        fgets(s, MAX, stdin);
        strip(s);
        addLast(historyList, buildNode_Type(buildTypeHistory(s)));

    }// end while

    // when "exit" command is given, save to .msh_history
    // from history list and alias list (and PATH..?)
    // -- code here --

    clearList(aliasList, cleanTypeAlias);
    free(aliasList);
    aliasList = NULL;

    clearList(historyList, cleanTypeHistory);
    free(historyList);
    historyList = NULL;

    printf("Silly Shell Ended\n");

    fclose(msh_history);
    fclose(mshrc);

    return 0;
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