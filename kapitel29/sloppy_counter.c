#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>

int numThreads = 0;
int numCount = 0;


typedef struct {
    int global_count;
    pthread_mutex_t global_lock;
    int local_count[50];
    pthread_mutex_t local_lock[50];
    int threshold;
}counter;

counter c1;

void init(counter *c, int threshold) {
    c->threshold = threshold;
    c->global_count = 0;
    pthread_mutex_init(&c->global_lock, NULL);
    for (int i = 0; i < numThreads; ++i) {
        c->local_count[i] = 0;
        pthread_mutex_init(&c->local_lock[i], NULL);
    }
}

void update(counter *c, int threadID) {
    int cpu = threadID % numCount;
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
    for (int i = 0; i < numCount; ++i) {
        update(&c1, threadID);
    }
    return NULL;
}

/* Sloppiness Testmethode
 * Sloppiness = von 1 bis 1024
 * numThreads = User Input
 * numCount = User Input
 */
void sloppiness_test() {
    pthread_t threads[numThreads];
    struct timeval start_tv;
    struct timeval end_tv;

    for (int j = 1; j < 1025; j = j * 2) {
        printf("Threshold: %d\n", j);
        init(&c1, j);
        gettimeofday(&start_tv, NULL);

        for (int i = 0; i < numThreads; ++i) {
            pthread_create(&threads[i], NULL, worker_counter, (void *) &i);
        }

        for (int i = 0; i < numThreads; ++i) {
            pthread_join(threads[i], NULL);
        }


        gettimeofday(&end_tv, NULL);

        printf("Value vom Counter = %d\n", get(&c1));
        printf("Millisekunden: %ld\n", (1000000 * (end_tv.tv_sec - start_tv.tv_sec) + (end_tv.tv_usec - start_tv.tv_usec)) / 1000);
    }
}

/*  normale Testmethode
 *  Sloppiness = 5
 *  numThreads = User Input
 *  numCount = User Input
 */
void normal_test() {
    pthread_t threads[numThreads];
    struct timeval start_tv;
    struct timeval end_tv;

    init(&c1, 5);
    gettimeofday(&start_tv, NULL);

    for (int i = 0; i < numThreads; ++i) {
        pthread_create(&threads[i], NULL, worker_counter, (void *) &i);
    }

    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end_tv, NULL);

    printf("Value vom Counter = %d\n", get(&c1));
    printf("Millisekunden: %ld\n", (1000000 * (end_tv.tv_sec - start_tv.tv_sec) + (end_tv.tv_usec - start_tv.tv_usec)) / 1000);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Benutzung: ./sloppy_counter Threadanzahl Countanzahl\n");
        return -1;
    }

    numThreads = atoi(argv[1]);
    numCount = atoi(argv[2]);

    //sloppiness_test();

    normal_test();

    return 0;
}