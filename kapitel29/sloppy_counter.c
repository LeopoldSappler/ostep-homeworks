#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

#define NUM_CPUS 12
#define NUM_COUNT 10000000
#define THRESHOLD 1000

typedef struct {
    int global_count;
    pthread_mutex_t global_lock;
    int local_count[NUM_CPUS];
    pthread_mutex_t local_lock[NUM_CPUS];
    int threshold;
}counter;

counter c1;

void init(counter *c, int threshold) {
    c->threshold = threshold;
    c->global_count = 0;
    pthread_mutex_init(&c->global_lock, NULL);
    for (int i = 0; i < NUM_CPUS; ++i) {
        c->local_count[i] = 0;
        pthread_mutex_init(&c->local_lock[i], NULL);
    }
}

void update(counter *c, int threadID) {
    int cpu = threadID % NUM_CPUS;
    pthread_mutex_lock(&c->local_lock[cpu]);
    c->local_count[cpu]++;
    if (c->local_count[cpu] >= c->threshold) {
        pthread_mutex_lock(&c->global_lock);
        c->global_count += c->local_count[cpu];
        pthread_mutex_unlock(&c->global_lock);
        c->local_count[cpu] = 0;
    }
    pthread_mutex_unlock(&c->local_lock[cpu]);
}

int get(counter *c) {
    pthread_mutex_lock(&c->global_lock);
    int retval = c->global_count;
    pthread_mutex_unlock(&c->global_lock);
    return retval;
}

void* worker_counter(void *arg) {
    int threadID = *((int *) arg);
    for (int i = 0; i < NUM_COUNT; ++i) {
        update(&c1, threadID);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_CPUS];
    struct timeval start_tv;
    struct timeval end_tv;

    //Test
    init(&c1, THRESHOLD);
    gettimeofday(&start_tv, NULL);

    for (int i = 0; i < NUM_CPUS; ++i) {
        pthread_create(&threads[i], NULL, worker_counter, (void *) &i);
    }

    for (int i = 0; i < NUM_CPUS; ++i) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end_tv, NULL);

    printf("Value vom Counter = %d\n", get(&c1));
    printf("Millisekunden: %ld\n", (1000000 * (end_tv.tv_sec - start_tv.tv_sec) + (end_tv.tv_usec - start_tv.tv_usec)) / 1000);
}