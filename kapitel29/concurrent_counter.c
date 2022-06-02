#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>

int numThreads = 0;
int numCount = 0; //Wie oft pro Thread der Counter inkrementiert wird


typedef struct {
    int value;
    pthread_mutex_t lock;
}counter;

counter c1; //Globaler Counter

void init(counter *c) {
    c->value = 0;
    pthread_mutex_init(&c->lock, NULL);
}

void increment(counter *c) {
    pthread_mutex_lock(&c->lock);
    c->value++;
    pthread_mutex_unlock(&c->lock);
}

// nicht benutzt
void decrement(counter *c) {
    pthread_mutex_lock(&c->lock);
    c->value--;
    pthread_mutex_unlock(&c->lock);
}

int get(counter *c) {
    pthread_mutex_lock(&c->lock);
    int retval = c->value;
    pthread_mutex_unlock(&c->lock);
    return retval;
}

void* worker_counter() {
    for (int i = 0; i < numCount; ++i) {
        increment(&c1);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Benutzung: ./concurrent_counter Threadanzahl Countanzahl\n");
        return -1;
    }

    numThreads = atoi(argv[1]);
    numCount = atoi(argv[2]);

    pthread_t threads[numThreads];
    struct timeval start_tv;
    struct timeval end_tv;

    init(&c1);
    gettimeofday(&start_tv, NULL);

    for (int i = 0; i < numThreads; ++i) {
        pthread_create(&threads[i], NULL, worker_counter, (void *) &i);
    }

    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end_tv, NULL);

    printf("Counter mit Threadanzahl: %d, Value vom Counter = %d\n", numThreads, get(&c1));
    printf("Millisekunden: %ld\n", (1000000 * (end_tv.tv_sec - start_tv.tv_sec) + (end_tv.tv_usec - start_tv.tv_usec)) / 1000);

    return 0;
}