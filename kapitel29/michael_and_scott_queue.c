#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_CPUS 8 //8 Cores

//structure copied by Ostep

typedef struct __node_t {
    int value;
    struct __node_t *next;
} node_t;

typedef struct __queue_t {
    node_t *head;
    node_t *tail;
    pthread_mutex_t head_lock, tail_lock;
} queue_t;

void Queue_Init(queue_t *q) {
    node_t *tmp = malloc(sizeof(node_t));
    tmp->next = NULL;
    q->head = q->tail = tmp;
    pthread_mutex_init(&q->head_lock, NULL); //lock head
    pthread_mutex_init(&q->tail_lock, NULL); //lock tail
}

void Queue_Enqueue(queue_t *q, int value) {
    node_t *tmp = malloc(sizeof(node_t));
    //assert(tmp != NULL);
    tmp->value = value;
    tmp->next = NULL;

    pthread_mutex_lock(&q->tail_lock);

    q->tail->next = tmp;
    q->tail = tmp;

    pthread_mutex_unlock(&q->tail_lock);
}

int Queue_Dequeue(queue_t *q) {
    pthread_mutex_lock(&q->head_lock);

    node_t *tmp = q->head;
    node_t *new_head = tmp->next;
    if (new_head == NULL) {

        pthread_mutex_unlock(&q->head_lock);
        return -1; // queue was empty
    }
    new_head = new_head->next;
    q->head = new_head;

    pthread_mutex_unlock(&q->head_lock);

    free(tmp);
    return 0;
}

static void *threadEnqueue(void *arg) {
    int listLength = 1000000;
    for (int i = 0; i < listLength; i++){
        Queue_Enqueue(arg, i);
    }
    pthread_exit(EXIT_SUCCESS);
}

static void *threadDequeue(void *arg) {
    int listLength = 1000000;
    for (int i = 0; i < listLength; i++){
        Queue_Dequeue(arg);
    }
    pthread_exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    struct timeval start_tv;
    struct timeval end_tv;

    pthread_t threads[NUM_CPUS];
    queue_t *queue = malloc(sizeof(queue_t));

    Queue_Init(queue);

    //Enqueue
    for(int i = 0; i < NUM_CPUS; i++) {
        gettimeofday(&start_tv, NULL);

        for (int j = 0; j <= i; j++) {
            pthread_create(&threads[i], NULL, &threadEnqueue, queue);
        }

        for (int j = 0; j <= i; j++) {
            pthread_join(threads[j], NULL);
        }

        gettimeofday(&end_tv, NULL);

        printf("Threads: %d\n", i + 1);
        printf("Michael and Scott Concurrent Queues Enqueue-Time in Millisekunden: %ld\n", (1000000 * (end_tv.tv_sec - start_tv.tv_sec) + (end_tv.tv_usec - start_tv.tv_usec)) / 1000);
    }

    printf("\n");

    //Dequeue
    for (int i = 0; i < NUM_CPUS; i++) {

        //Initialisieren und Fuellen der Queue
        Queue_Init(queue);
        for (int j = 0; j <= i; j++) {
            pthread_create(&threads[i], NULL, &threadEnqueue, queue);
        }

        for (int j = 0; j <= i; j++) {
            pthread_join(threads[j], NULL);
        }

        //Dequeue Messung
        gettimeofday(&start_tv, NULL);

        for (int j = 0; j <= i; j++) {
            pthread_create(&threads[j], NULL, &threadDequeue, queue);
        }

        for (int j = 0; j <= i; j++) {
            pthread_join(threads[j], NULL);
        }

        gettimeofday(&end_tv, NULL);

        printf("Threads: %d\n", i + 1);
        printf("Michael and Scott Concurrent Queues Dequeue-Time in Millisekunden: %ld\n", (1000000 * (end_tv.tv_sec - start_tv.tv_sec) + (end_tv.tv_usec - start_tv.tv_usec)) / 1000);
    }
    return 0;
}

