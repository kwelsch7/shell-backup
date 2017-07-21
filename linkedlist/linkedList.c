#include "linkedList.h"

LinkedList * linkedList()
{
    LinkedList * theList = (LinkedList *) calloc(1, sizeof(LinkedList));
    theList->head = (Node *) calloc(1, sizeof(Node));
    theList->tail = (Node *) calloc(1, sizeof(Node));
    theList->size = 0;

    theList->head->next = theList->tail;
    theList->tail->prev = theList->head;

    return theList;
}

void addLast(LinkedList * theList, Node * nn)
{
    if(theList == NULL || nn == NULL)
    {
        perror("Null parameter(s) to addLast\n");
        exit(-99);
    }

    nn->next = theList->tail;
    nn->prev = theList->tail->prev;
    theList->tail->prev->next = nn;
    theList->tail->prev = nn;
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

Node * getSecondToLast(const LinkedList * theList)
{
    if(theList == NULL)
    {
        perror("Null parameter to getLast\n");
        exit(-99);
    }
    if(theList->size == 0)
    {
        printf("Empty list to getLast");
    }
    if(theList->size == 1)
    {
        printf("Only one item in list");
    }
    else
    {
        return theList->tail->prev->prev;
    }
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
        Node * oldNode = theList->tail->prev;
        theList->tail->prev = theList->tail->prev->prev;
        theList->tail->prev->next = theList->tail;
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
        while(cur != theList->tail && compare(cur->data, nn->data) != 0)
        {
            cur = cur->next;
        }
        if(cur == theList->tail)
        {
            printf("Not found\n");
        }
        else
        {
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;
            cur->next = NULL;
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

        while(cur != theList->tail)
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
        free(theList->tail);
        theList->tail = NULL;
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
        while(cur != theList->tail)
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
        //printf("Empty list to searchList\n");
    }
    else
    {
        Node * cur = theList->head->next;
        while(cur != theList->tail && compare(cur->data, nn->data) != 0)
        {
            cur = cur->next;
        }
        if(cur != theList->tail)
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