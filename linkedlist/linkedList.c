#include "linkedList.h"

LinkedList * linkedList()
{
    LinkedList * theList = (LinkedList *) calloc(1, sizeof(LinkedList));
    theList->head = (Node *) calloc(1, sizeof(Node));
    theList->size = 0;

    return theList;
}

void addLast(LinkedList * theList, Node * nn)
{
    if(theList == NULL || nn == NULL)
    {
        perror("Null parameter(s) to addLast\n");
        exit(-99);
    }

    Node * cur = theList->head;
    for( ; cur->next != NULL; cur = cur->next);

    cur->next = nn;
    nn->prev = cur;
    theList->size++;
}

void addFirst(LinkedList * theList, Node * nn)
{
    if(theList == NULL || nn == NULL)
    {
        perror("Null parameter(s) to addfirst\n");
        exit(-99);
    }

    nn->prev = theList->head;
    nn->next = theList->head->next;
    theList->head->next->prev = nn;
    theList->head->next = nn;
    theList->size++;
}

void removeFirst(LinkedList * theList, void (*removeData)(void *))
{
    if(theList == NULL)
    {
        perror("Null list to removeFirst\n");
        exit(-99);
    }

    if(theList->size == 0)
    {
        printf("Empty list -- can't removeFirst\n");
    }
    else
    {
        Node * oldNode = theList->head->next;
        theList->head->next = theList->head->next->next;
        if(theList->size > 1)
            theList->head->next->prev = theList->head;
        oldNode->prev = NULL;
        oldNode->next = NULL;
        removeData(oldNode->data);
        free(oldNode->data);
        oldNode->data = NULL;
        free(oldNode);
        oldNode = NULL;
        theList->size--;
    }
}

void removeLast(LinkedList * theList, void (*removeData)(void *))
{
    if(theList == NULL)
    {
        perror("Null list to removeLast\n");
        exit(-99);
    }
    
    if(theList->size == 0)
    {
        printf("Empty list -- can't removeLast\n");
    }
    else
    {
        Node * cur = theList->head;
        for( ; cur->next != NULL; cur = cur->next);
        cur->prev->next = NULL;
        cur->prev = NULL;
        removeData(cur->data);
        free(cur->data);
        cur->data = NULL;
        free(cur);
        cur = NULL;
        theList->size--;
    }
}

void removeItem(LinkedList * theList, Node * nn, void (*removeData)(void *), int (*compare)(const void *, const void *))
{
    if(theList == NULL || nn == NULL)
    {
        perror("Null parameter(s) to removeItem\n");
        exit(-99);
    }
    
    if(theList->size == 0)
    {
        printf("Empty list -- can't removeItem\n");
    }
    else
    {
        Node * cur = theList->head->next;
        while(cur != NULL && compare(cur->data, nn->data) != 0)
        {
            cur = cur->next;
        }
        if(cur == NULL)
        {
            printf("Not found\n");
        }
        else
        {
            cur->prev->next = cur->next;
            if(cur->next != NULL)
            {
                cur->next->prev = cur->prev;
                cur->next = NULL;
            }
            cur->prev = NULL;
            removeData(cur->data);
            free(cur->data);
            cur->data = NULL;
            free(cur);
            cur = NULL;
            theList->size--;

            printf("Successfully removed\n");
        }
    }
    removeData(nn->data);
    free(nn->data);
    nn->data = NULL;
    free(nn);
    nn = NULL;
}

void clearList(LinkedList * theList, void (*removeData)(void *))
{
    if(theList != NULL)
    {
        Node * cur = theList->head->next;
        Node * prev;

        while(cur != NULL)
        {
            prev = cur;
            cur = cur->next;
            removeData(prev->data);
            free(prev->data);
            prev->data = NULL;
            free(prev);
            prev = NULL;
        }

        free(theList->head);
        theList->head = NULL;
    }
}

void printList(const LinkedList * theList, void (*convertData)(void *))
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
        Node * cur = theList->head->next;
        while(cur != NULL)
        {
            convertData(cur->data);
            cur = cur->next;
        }
    }
}

Node * searchList(LinkedList * theList, Node * nn, void (*removeData)(void *), int (*compare)(const void *, const void *))
{
    if(theList == NULL || nn == NULL)
    {
        perror("Null parameter(s) to searchList\n");
        exit(-99);
    }

    Node * foundNode = NULL;

    if(theList->size == 0)
    {
        printf("Empty list to searchList\n");
    }
    else
    {
        Node * cur = theList->head->next;
        while(cur != NULL && compare(cur->data, nn->data) != 0)
        {
            cur = cur->next;
        }
        if(cur != NULL)
        {
            foundNode = cur;
        }
    }

    removeData(nn->data);
    free(nn->data);
    nn->data = NULL;
    free(nn);
    nn = NULL;

    return foundNode;
}