#include <stdio.h>
#include <semaphore.h>

typedef struct mainNode
{
    struct mainNode *next;
    struct Node *down;
    unsigned int hash;
}mainNode;

typedef struct List{
    mainNode* first;
    sem_t* semaphore;
    void* hashFunc;
}List;

List* createList(void* hashFunc);
void addElement(List* list, void *node_data, char* type, int typeSize);
void print(List* list, char* type);
void deleteElement(List* list, unsigned int hash);
void deleteAll(List* list);