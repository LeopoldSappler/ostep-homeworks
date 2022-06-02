#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_CPUS 8 //8 Cores

//Basic linked List Implementation copied by https://www.geeksforgeeks.org/linked-list-set-1-introduction/ and Ostep

// Node
typedef struct node{
    int data;
    pthread_mutex_t nodeLock;
    struct node* next;
}node;

// List
typedef struct {
    node *head;
    pthread_mutex_t  listLock;
}list;

list list1;

void init(list *list){
    list->head = NULL;
    pthread_mutex_init(&list->listLock, NULL);
}

void insert(list *list, int val) {
    node *newNode = malloc(sizeof(node));
    newNode->data = val;

    pthread_mutex_init(&newNode-> nodeLock, NULL);
    pthread_mutex_lock(&list->listLock);

    newNode->next = list->head;
    list->head = newNode;

    pthread_mutex_unlock(&list->listLock);
}

int find(list *list, int val){ //traversing the List
    node *current = list->head;

    pthread_mutex_lock(&current->nodeLock);
    while(current->next != NULL) {
        if (current->data == val) {
            //printf("Element gefunden");
            pthread_mutex_unlock(&current->nodeLock);
            return 0; //success
        }
        node *next = current->next; // first grab next node
        if(next->next != NULL){
            pthread_mutex_lock(&next->nodeLock); //lock next node
            //printf("next node locked!!!");
        }
        pthread_mutex_unlock(&current->nodeLock); //release current node
        current = next;
    }
    return -1; //failure
}

void *worker_insert(void *arg) {
    list *list = arg;
    int listLength = 1000000;
    for (int i = 0; i < listLength; i++){
        insert(list, i);
    }
    return NULL;
}

void *worker_find(void *arg) {
    list *list = arg;
    find(list, 0);
    return NULL;
}

int main(void) {
    struct timeval start_tv;
    struct timeval end_tv;

    pthread_t threads[NUM_CPUS];

    //Insert into List
    for(int i = 0; i < NUM_CPUS; i++) {
        init(&list1);
        gettimeofday(&start_tv, NULL);

        for (int j = 0; j <= i; j++) {
            pthread_create(&threads[j], NULL, &worker_insert, &list1);
        }

        for (int j = 0; j <= i; j++) {
            pthread_join(threads[j], NULL);
        }

        gettimeofday(&end_tv, NULL);

        printf("Threads: %d\n", i + 1);
        printf("Hand-Over-Hand List Insert-Time in Millisekunden: %ld\n", (1000000 * (end_tv.tv_sec - start_tv.tv_sec) + (end_tv.tv_usec - start_tv.tv_usec)) / 1000);
    }
    printf("\n");

    //Lookup List
    for (int i = 0; i < NUM_CPUS; i++) {
        gettimeofday(&start_tv, NULL);

        for (int j = 0; j < i; j++) {
            pthread_create(&threads[j], NULL, &worker_find, &list1);
        }

        for (int j = 0; j < i; j++) {
            pthread_join(threads[j], NULL);
        }

        gettimeofday(&end_tv, NULL);

        printf("Threads: %d\n", i);
        printf("Hand-Over-Hand List Lookup-Time in Millisekunden: %ld\n", (1000000 * (end_tv.tv_sec - start_tv.tv_sec) + (end_tv.tv_usec - start_tv.tv_usec)) / 1000);
    }
    return 0;
}



