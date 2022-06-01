#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

const int NUM_COUNT = 10000000; //Wie oft pro Thread der Counter inkrementiert wird

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
    for (int i = 0; i < NUM_COUNT; ++i) {
        increment(&c1);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2, t3, t4;
    struct timeval start_tv;
    struct timeval end_tv;

    //Counter mit 1 Thread
    init(&c1);
    gettimeofday(&start_tv, NULL);
    pthread_create(&t1, NULL, worker_counter, NULL);
    pthread_join(t1, NULL);
    gettimeofday(&end_tv, NULL);

    printf("Counter mit 1 Thread: Value vom Counter = %d\n", get(&c1));
    printf("Millisekunden: %ld\n", (1000000 * (end_tv.tv_sec - start_tv.tv_sec) + (end_tv.tv_usec - start_tv.tv_usec)) / 1000);

    //Counter mit 2 Threads
    init(&c1);
    gettimeofday(&start_tv, NULL);
    pthread_create(&t1, NULL, worker_counter, NULL);
    pthread_create(&t2, NULL, worker_counter, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    gettimeofday(&end_tv, NULL);

    printf("Counter mit 2 Threads: Value vom Counter = %d\n", get(&c1));
    printf("Millisekunden: %ld\n", (1000000 * (end_tv.tv_sec - start_tv.tv_sec) + (end_tv.tv_usec - start_tv.tv_usec)) / 1000);

    //Counter mit 3 Threads
    init(&c1);
    gettimeofday(&start_tv, NULL);
    pthread_create(&t1, NULL, worker_counter, NULL);
    pthread_create(&t2, NULL, worker_counter, NULL);
    pthread_create(&t3, NULL, worker_counter, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    gettimeofday(&end_tv, NULL);

    printf("Counter mit 3 Threads: Value vom Counter = %d\n", get(&c1));
    printf("Millisekunden: %ld\n", (1000000 * (end_tv.tv_sec - start_tv.tv_sec) + (end_tv.tv_usec - start_tv.tv_usec)) / 1000);

    //Counter mit 4 Threads
    init(&c1);
    gettimeofday(&start_tv, NULL);
    pthread_create(&t1, NULL, worker_counter, NULL);
    pthread_create(&t2, NULL, worker_counter, NULL);
    pthread_create(&t3, NULL, worker_counter, NULL);
    pthread_create(&t4, NULL, worker_counter, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    gettimeofday(&end_tv, NULL);

    printf("Counter mit 4 Threads: Value vom Counter = %d\n", get(&c1));
    printf("Millisekunden: %ld\n", (1000000 * (end_tv.tv_sec - start_tv.tv_sec) + (end_tv.tv_usec - start_tv.tv_usec)) / 1000);
}