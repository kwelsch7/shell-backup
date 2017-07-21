#include "history.h"

void cleanTypeHistory(void * ptr)
{
    if(ptr == NULL)
    {
        perror("Null parameter to cleanTypeHistory\n");
        exit(-99);
    }

    free(((History *)ptr)->fullLine);
    ((History *)ptr)->fullLine = NULL;
}

void * buildTypeHistory(char * command)
{
    if(command == NULL)
    {
        perror("Null parameter to buildTypeHistory\n");
        exit(-99);
    }

    History * hist = (History *) calloc(1, sizeof(History));
    hist->fullLine = (char *) calloc(strlen(command) + 1, sizeof(char));
    strncpy(hist->fullLine, command, strlen(command));

    return hist;
}

void printTypeHistory(int count, void * passedIn)
{
    if(passedIn == NULL)
    {
        perror("Null parameter to printTypeHistory\n");
        exit(-99);
    }

    printf("  %d  %s\n", count, ((History *)passedIn)->fullLine);
}

void printHistoryList(const LinkedList * theList, int histCount)
{
    if(theList == NULL)
    {
        perror("Null list to printList\n");
        exit(-99);
    }
    if(theList->size == 0)
    {
        printf("Empty list to printList\n");
    }
    else
    {
        int count = 1;
        Node * cur = theList->head->next;

        if(theList->size > histCount)
        {
            int startingPoint = theList->size - histCount;
            while(cur != theList->tail && count <= startingPoint)
            {
                cur = cur->next;
                count++;
            }
            count = 1;
        }

        while(cur != theList->tail && count <= histCount)
        {
            printTypeHistory(count, cur->data);
            cur = cur->next;
            count++;
        }
    }
}