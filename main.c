#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "hash.h"

#define NUMBER_OF_ELEMENTS 100 //elemens to be added in each thread
#define THREADS 10 //has to be a multiple of 2; 1 for adding, 1 for deleting

typedef struct args{
    List* list;
    void* value;
    char* type;
    int typeSize;
    int toBeAdded;
}args;

unsigned int hashFunc(void* value, int max_bytes) {
    unsigned int hash_val = 0;
    char* ptr = (char*) value;
    
    for (int i = 0; i < max_bytes && *ptr; i++) {
        hash_val = ((hash_val << 5) + hash_val) + (*ptr);
        ptr++;
    }
    
    return hash_val;
}

void addSome(args* args){
    for(int i=0;i<NUMBER_OF_ELEMENTS;i++){
        int *temp = (int *)malloc(sizeof(int));     
        *temp = args->toBeAdded;
        args->value = temp;    
        addElement(args->list, args->value,args->type,args->typeSize);
    }
}

void deleteSome(args* args){
    for(int i=0;i<NUMBER_OF_ELEMENTS;i++){      
        usleep(100);
        deleteElement(args->list,args->toBeAdded);
    }
}

int main(int argc, char **argv)
{
    int quit = 0;
    List* list = createList(hashFunc);
    char answer,type[6]={'i','n','t'};
    
    int *temp = (int *)malloc(sizeof(int));     
    *temp = 15392832;
    addElement(list, temp, type, sizeof(int));    

    args Args[THREADS];
    pthread_t threads[THREADS];

    for(int i=0;i<THREADS/2;i++){
        Args[i].toBeAdded = i; 
        Args[i].list=list;
        Args[i].type=type;
        Args[i].typeSize=sizeof(int);
        pthread_create(&threads[2*i],NULL, (void*)addSome, (void*)&Args[i]);
        pthread_create(&threads[2*i+1],NULL, (void*)deleteSome, (void*)&Args[i]);
    }

    for(int i=0;i<THREADS/2;i++){
        pthread_join(threads[2*i],NULL); // Watki dodajace
        pthread_join(threads[2*i+1],NULL); // Watki usuwajace
    }
 

    print(list, type);
    //printf("%d\n",addedElements);
    return 0;
}
