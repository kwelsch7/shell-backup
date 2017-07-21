#include "./utils/shellUtils.h"

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
    char s[MAX];

    printf("command?: ");
    fgets(s, MAX, stdin);
    strip(s);
    addLast(historyList, buildNode_Type(buildTypeHistory(s)));
    //need to start removing first if after histFileCount!!!!!!!!

    while(doCommand(s, historyList, aliasList, histCount) != -1)
    {
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
