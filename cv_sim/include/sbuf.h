#ifndef __SBUF_H__
#define __SBUF_H__

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

/* $begin sbuft */
typedef struct {
    int *buffer;
    int *id;
    int number;
    int front;
    int rear;
    pthread_mutex_t mutex;
    pthread_cond_t condition;
} sbuf_t;
/* $end sbuft */

void sbuf_init(sbuf_t *sp, int n);
void sbuf_deinit(sbuf_t *sp);
void sbuf_insert(sbuf_t *sp, int item, char *msg);
int sbuf_remove(sbuf_t *sp, char *msg);

#endif /* __SBUF_H__ */
