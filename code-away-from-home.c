else if(strstr(s, "|") != NULL)
{
    char * copy = (char *) calloc(strlen(s) + 1, sizeof(char));
    char * copyPointer = copy; // for keeping original location of copy after it gets mangled, so it can be freed
    strncpy(copy, s, strlen(s));

    // check if I need to trim these
    char * firstCommand = strtok_r(copy, "|", &copy);
    char * secondCommand = strtok_r(NULL, "|", &copy);

    if(firstCommand[0] == '!')
    {

    }
    // more to write

    free(copyPointer);
    copyPointer = NULL;
}
else if(strstr(s, "!") == s)
{
    // just go replace the command and call doCommand on it
    char * actualCommand = getCommandFromHistory(s, historyList, histCount);
    doCommand(actualCommand, historyList, aliasList, histCount);
}

// ...later in file...

char * getCommandFromHistory(char * bangNumber, LinkedList * historyList, int histCount)
{
    if(historyList == NULL)
    {
        perror("Null list to getCommandFromHistory\n");
        exit(-99);
    }
    if(theList->size <= 1)  // the 1 because the ! command itself is stored (as far as this assignment goes)
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
