#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

int num_count = 10000; //Wie oft pro Thread der Counter inkrementiert wird

typedef struct {
    int value;
    pthread_mutex_t lock;
}counter;

counter c; //Globaler Counter

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
    for (int i = 0; i < num_count; ++i) {
        increment(&c);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2, t3, t4;
    struct timeval start_tv;
    struct timeval end_tv;

    //Counter mit 1 Thread
    init(&c);
    gettimeofday(&start_tv, NULL);
    pthread_create(&t1, NULL, worker_counter, NULL);
    pthread_join(t1, NULL);
    gettimeofday(&end_tv, NULL);

    //Counter mit 2 Threads
    init(&c);
    gettimeofday(&start_tv, NULL);
    pthread_create(&t1, NULL, worker_counter, NULL);
    pthread_create(&t2, NULL, worker_counter, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    gettimeofday(&end_tv, NULL);

    //Counter mit 3 Threads

    printf("Counter mit 2 Threads -> Sekunden: %ld, Mikrosekunden: %ld\n", end_tv.tv_sec - start_tv.tv_sec, end_tv.tv_usec - start_tv.tv_usec);
}