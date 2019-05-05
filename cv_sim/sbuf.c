#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#include "common_threads.h"
#include "sbuf.h"

void sbuf_init(sbuf_t *sp, int n) {
    void *p;
    assert((p = calloc(n, sizeof(int))) != NULL);
    sp->buffer = p;
    assert((p = calloc(n, sizeof(char))) != NULL);
    sp->id = p;
    sp->number = n;           // Queue have the most n items
    sp->front = sp->rear = 0; // Empty Queue
}
void sbuf_deinit(sbuf_t *sp) { free(sp->buffer); }
void sbuf_insert(sbuf_t *sp, int item_number, char *msg) {
    sp->rear++; // Extend queue size
    int index = sp->rear % sp->number;
    sp->buffer[index] = item_number; /* Insert item */
    sp->id[index] = msg[0];

    printf("I am a PRODUCER # %c inserting item %d\n", msg[0], item_number);
    printf("Number of items in queue now is %d\n", sp->rear - sp->front);
}
int sbuf_remove(sbuf_t *sp, char *msg) {
    sp->front++; // Decrease queue size
    int index = sp->front % sp->number;
    int item = sp->buffer[index]; /* Remove item */

    printf("I am a consumer # %c consuming item %d from producer # %c\n",
           msg[0], item, sp->id[index]);
    printf("Number of items in queue now is %d\n", sp->rear - sp->front);
}
