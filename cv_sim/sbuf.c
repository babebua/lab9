#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#include "common_threads.h"
#include "sbuf.h"

void sbuf_init(sbuf_t *sp, int n) {
    void *p;
    assert((p = calloc(n, sizeof(int))) != NULL);
    sp->buf = p;
    assert((p = calloc(n, sizeof(char))) != NULL);
    sp->id = p;
    sp->n = n;                /* Buffer holds max of n items */
    sp->front = sp->rear = 0; /* Empty buffer iff front == rear */
}
void sbuf_deinit(sbuf_t *sp) { free(sp->buf); }
void sbuf_insert(sbuf_t *sp, int item, char *msg) {
    int index;
    index = (++sp->rear) % (sp->n);
    sp->buf[index] = item; /* Insert the item */
    sp->id[index] = msg[0];

    printf("I am a PRODUCER # %c inserting item %d from PRODUCER # %c\n",
           sp->id[index], item, sp->id[index]);
    printf("Number of items in queue now is %d\n", sp->rear - sp->front);
}
int sbuf_remove(sbuf_t *sp, char *msg) {
    int item, index;
    index = (++sp->front) % (sp->n);
    item = sp->buf[index]; /* Remove the item */

    printf("I am a consumer # %c consuming item %d from producer # %c\n",
           msg[0], item, sp->id[index]);
    printf("Number of items in queue now is %d\n", sp->rear - sp->front);
}
