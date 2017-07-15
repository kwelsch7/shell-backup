#include "./linkedlist/linkedList.h"
#include "./linkedlist/listUtils.h"
#include "./pipes/pipes.h"
#include "./utils/myUtils.h"
#include "./process/process.h"
#include "./tokenize/makeArgs.h"
#include "./alias/alias.h"
#include "./history/history.h"

int main()
{
    // check .mshrc -- create it if necessary w/ default values
    // create/fill history linkedList
    // create/fill alias linkedList
    LinkedList * historyList = linkedList();
    LinkedList * aliasList = linkedList();
    FILE * mshrc = fopen(".mshrc", "r+");
    FILE * msh_history = fopen(".msh_history", "r+");
    
    if(mshrc == NULL)
    {
        mshrc = fopen(".mshrc", "w+");
        fprintf(mshrc, "%s%d\n", "HISTCOUNT=", 100);
        fprintf(mshrc, "%s%d\n", "HISTFILECOUNT=", 1000);
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

    while(strcmp(s, "exit") != 0)
    {
        if(strcmp(s, "history") == 0)
        {
            printList(historyList, printTypeHistory);
        }
        else if(strstr(s, "alias") != NULL)
        {
            if(strcmp(s, "alias") == 0) // alias IS the string
            {
                printList(aliasList, printTypeAlias);
            }
            else if(strstr(s, "alias ") == s) // alias at beginning of line
            {
                // check if valid maybe (the format 'alias ali="command"', not the command itself), then save entire s to alias->command
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

        }

        else  // perform command as normal
        {
            //add to history (even if invalid or duplicate)

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

                    // I think I need to do a makeargs or something here to save it to history... or just save prePipe, "|", and postPipe?

                }// end if pipeCount

                else
                {
                    argc = makeargs(s, &argv);
                    if(argc != -1)
                    {
                        // save to history
                        
                        forkIt(argv);
                        clean(argc, argv);
                        argv = NULL;
                    }// end if
                }
            }

            free(copyPointer);
            copyPointer = NULL;
        }

        // Save whatever it was in history

        printf("command?: ");
        fgets(s, MAX, stdin);
        strip(s);

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

    fclose(mshrc);

    return 0;
}