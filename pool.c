#include "pool.h"

static void *worker_loop(void *arg) {
  struct context *ctx = (struct context *)arg;
  while (!ctx->done) {
    work w = work_queue_pop(&ctx->wq);
    w.cb(w.data);
  }
  return NULL;
}

threadpool threadpool_init(struct context *ctx, ssize_t n) {
  worker *workers = (worker *)malloc(sizeof(worker) * (size_t)n);
  for (ssize_t i = 0; i < n; ++i) {
    pthread_create(&(workers[i]), NULL, worker_loop, ctx);
  }
  return (threadpool){
      .ctx = ctx,
      .workers = workers,
      .workers_len = n,
  };
}

void threadpool_enqueue(threadpool *pool, work w) {
  work_queue_push(&pool->ctx->wq, w);
}

void threadpool_deinit(threadpool *pool) {
  pool->ctx->done = 1;
  for (ssize_t i = 0; i < pool->workers_len; ++i) {
    pthread_join(pool->workers[i], NULL);
  }
  free(pool->workers);
  pool->workers = NULL;
  pool->workers_len = -1;
}
