#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "hash.h"

typedef struct Node
{
    struct Node *next;
    void *data;
}Node;


List* createList(void* hashFunc){
    List* list = (List*)malloc(sizeof(List));
    list->semaphore = (sem_t*)malloc(sizeof(sem_t));
    sem_init(list->semaphore, 0, 1);
    list->first = NULL;
    list->hashFunc = hashFunc;
    return list;
}

void addElement(List* list, void *node_data, char* type, int typeSize)
{
    unsigned int (*func)(void*, int) = list->hashFunc;
    unsigned int hash = (*func)(node_data, typeSize);
    sem_wait(list->semaphore);
    //no elements exist
    if (list->first == NULL)
    {
        mainNode* newNode = (mainNode*)malloc(sizeof(mainNode));
        list->first = newNode;
        newNode->hash = hash;
        newNode->next=NULL;
        newNode->down = malloc(sizeof(Node));
        newNode->down->data = node_data;
        newNode->down->next=NULL;
          sem_post(list->semaphore);
        return;
    }

    else
    {
        // Searching for a node with that hash
        int isThere = 0;
        mainNode *temp = list->first;
        if (temp->hash == hash) isThere = 1;
        if(!isThere){while (temp->next != NULL && isThere==0)
        {
            temp = temp->next;
            if (temp->hash == hash)
            {
                isThere = 1;
            }
        }
        }

        if (isThere)
        {
            // If node with that hash exists
            Node *temp2 = (temp->down);
            if(temp2==NULL){
                Node *newNode = malloc(sizeof(Node));
                temp->down=newNode;
                temp2 = newNode;
                newNode->data = node_data;
                newNode->next=NULL;
            }
            else if (temp2->next == NULL)
            {
                Node *newNode = malloc(sizeof(Node));
                temp2->next = newNode;
                newNode->data = node_data;
                newNode->next=NULL;
            }
            else
            {
                while (temp2->next != NULL)
                {
                    temp2 = temp2->next;
                }
                Node *newNode = malloc(sizeof(Node));
                temp2->next = newNode;
                newNode->data = node_data;
                newNode->next=NULL;
            }
        }

        else
        {
            mainNode* newNode =malloc(sizeof(mainNode));
            temp->next=newNode;
            newNode->next=NULL;
            newNode->hash=hash;
            newNode->down=malloc(sizeof(Node));
            newNode->down->data = node_data;
            newNode->down->next=NULL;
        }
    }
      sem_post(list->semaphore);
}

void printNode(Node *node, char* type)
{
    if (node != NULL )
    {
        if (strcmp(type, "int") == 0)
        {
            printf("%d\n", (*(int *)node->data));
        }
        else if (strcmp(type, "double") == 0)
        {
            printf("%lf\n", *(double *)node->data);
        }
         else if (strcmp(type, "string") == 0)
        {
            char *temp = (char*)node->data;
            printf("%s\n", temp);
        }
         else if (strcmp(type, "float") == 0)
        {
            printf("%f\n", *(float *)node->data);
        }
         else if (strcmp(type, "uint") == 0)
        {
            printf("%u\n", *(unsigned *)node->data);
        }
        if(node->next!=NULL) printNode(node->next, type);
    }
    return;
}

void print(List* list, char* type)
{
    if (list->first == NULL)
    {
        printf("Hash table is empty.\n");
        return;
    }
    mainNode* head = list->first;
    while (head != NULL)
    {
        if (head->down != NULL)
        {
            printf("Node nr: %u\n", head->hash);
            printNode(head->down, type);
        }
        head = head->next;
    }
}

void deleteElement(List* list, unsigned int hash)
{
    sem_wait(list->semaphore);
    mainNode* prev2 = NULL;
    mainNode* curr = list->first;
    bool nodeFound = false;

    while (curr != NULL)
    {
        if (curr->hash == hash)
        {
            nodeFound = true;

            if (curr->down == NULL)
            {
                if (curr == list->first)
                {
                    // Removing the first node in the list
                    list->first = curr->next;
                    free(curr);

                }
                else
                {
                    // Removing a node that is not the first in the list
                    prev2->next = curr->next;
                    free(curr);
                }
            }
            else if (curr->down->next == NULL) // Only one down element
            {
                // Remove the only down node
                free(curr->down->data);
                free(curr->down);
                curr->down = NULL;
           
                // Remove the main node
                if (curr == list->first)
                {
                    
                    // Removing the first node in the list
                    list->first = curr->next;
                    free(curr);
                }
                else
                {
                   
                    // Removing a node that is not the first in the list
                    prev2->next = curr->next;
                    free(curr);
                }
            }
            else
            {
                // Remove the last node in the down list
                Node* currNode = curr->down;
                Node* prevNode = NULL;
                while (currNode->next != NULL)
                {
                    prevNode = currNode;
                    currNode = currNode->next;
                }
                free(currNode->data);
                free(currNode);
                if (prevNode == NULL)
                {
                    curr->down = NULL;
                }
                else
                {
                    prevNode->next = NULL;
                }
            }
            curr = NULL; 
        }
        else
        {
           
            prev2 = curr;
            curr = curr->next;
        }
    }

    if (!nodeFound)
    {
        printf("Node with hash value %u does not exist.\n", hash);
    }
    sem_post(list->semaphore);
}

void deleteAll(List* list){

    while(list->first!=NULL){
        mainNode* curr = list->first;
        mainNode* prev = list->first;
        while(curr->next!=NULL){
        prev = curr;    
        curr=curr->next;
        }
        if(prev==list->first){
            deleteElement(list,list->first->hash);
            return;
        }
        prev->next=NULL;
        while(curr!=NULL){
            deleteElement(list, curr->hash);}
            
        }
}
