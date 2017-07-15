#include "listUtils.h"

Node * buildNode(FILE * fin, void *(*buildData)(FILE * in) )
{
    Node * nn = (Node *) calloc(1, sizeof(Node));
    nn->data = buildData(fin);

    return nn;
}

Node * buildNode_Type(void * passedIn)
{
    Node * nn = (Node *) calloc(1, sizeof(Node));
    nn->data = passedIn;

    return nn;
}

void sort(LinkedList * theList, int (*compare)(const void *, const void *))
{
    if(theList == NULL)
    {
        perror("Null parameter to sort\n");
        exit(-99);
    }

    Node * start, * search, * min;
    void * temp;

    if(theList->size > 1)
    {
        for(start = theList->head->next; start->next != NULL; start = start->next)
        {
            min = start;
            for(search = start->next; search != NULL; search = search->next)
            {
                if(compare(search->data, min->data) < 0)
                {
                    min = search;
                }
            }
            temp = min->data;
            min->data = start->data;
            start->data = temp;
        }
    }
}

void buildListTotal(LinkedList * myList, int total, FILE * fin, void * (*buildData)(FILE * in))
{
    if(myList == NULL)
    {
        perror("Null parameter to buildListTotal\n");
        exit(-99);
    }
    if(total <= 0)
    {
        perror("Total passed to buildListTotal must be positive\n");
        exit(-99);
    }

    Node * cur = myList->head;
    Node * nn;
    while(myList->size < total)
    {
        nn = buildNode(fin, buildData);
        cur->next = nn;
        nn->prev = cur;
        cur = cur->next;
        myList->size++;
    }
}