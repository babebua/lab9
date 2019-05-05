#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common_threads.h"
#include "sbuf.h"

sbuf_t *shared_data;

void *producer(void *arg) {
    char *msg = arg;
    int item_number = 1;
    while (1) {
        Pthread_mutex_lock(&(shared_data->mutex));

        while (shared_data->rear - shared_data->front >= shared_data->number)
            Pthread_cond_wait(&(shared_data->condition),
                                   &(shared_data->mutex));

        sbuf_insert(shared_data, item_number, msg);
        item_number++;
        Pthread_cond_signal(&(shared_data->condition));

        Pthread_mutex_unlock(&(shared_data->mutex));
    }

    return NULL;
}

void *consumer(void *arg) {
    char *msg = arg;
    while (1) {
        Pthread_mutex_lock(&(shared_data->mutex));

        while (shared_data->rear - shared_data->front <= 0)
            Pthread_cond_wait(&(shared_data->condition),
                                   &(shared_data->mutex));

        sbuf_remove(shared_data, msg);
        Pthread_cond_signal(&(shared_data->condition));

        Pthread_mutex_unlock(&(shared_data->mutex));
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p1, p2, p3, p4;

    printf("size of sd = %lu\n", sizeof(sbuf_t));
    shared_data = malloc(sizeof(sbuf_t));
    if (shared_data == NULL) {
        fprintf(stderr, "Malloc unsuccessful\n");
        exit(0);
    }

    // Initialize buffer
    sbuf_init(shared_data, 5);

    Pthread_create(&p1, NULL, consumer, "1");
    Pthread_create(&p2, NULL, consumer, "2");
    Pthread_create(&p3, NULL, producer, "1");
    Pthread_create(&p4, NULL, producer, "2");

    // join waits for the threads to finish
    Pthread_join(p1, NULL);
    Pthread_join(p2, NULL);
    Pthread_join(p3, NULL);
    Pthread_join(p4, NULL);

    return 0;
}
