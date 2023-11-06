#ifndef POOL_H_
#define POOL_H_

#include "queue.h"
#include <pthread.h>
#include <stdlib.h>

struct context {
  work_queue wq;
  _Atomic int done;
};

typedef pthread_t worker;

typedef struct {
  struct context *ctx;
  worker *workers;
  ssize_t workers_len;
} threadpool;

threadpool threadpool_init(struct context *ctx, ssize_t n);
void threadpool_enqueue(threadpool *pool, work w);
void threadpool_deinit(threadpool *pool);

#endif
