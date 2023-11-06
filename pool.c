#include "pool.h"
#include <pthread.h>
#include <stdio.h>

static int counter = 0;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static void *
worker_loop(void *arg)
{
    struct context *ctx = (struct context *) arg;
    for (;;) {
        work w = work_queue_pop(&ctx->wq);
	if (w.type == TYPE_STOP) {
		break;
	}
        w.cb(w.data);
    }
    pthread_mutex_lock(&lock);
    counter++;
    pthread_mutex_unlock(&lock);
    return NULL;
}

threadpool
threadpool_init(struct context *ctx, ssize_t n)
{
    worker *workers = (worker *) malloc(sizeof(worker) * (size_t) n);
    for (ssize_t i = 0; i < n; ++i) {
        pthread_create(&(workers[i]), NULL, worker_loop, ctx);
    }
    return (threadpool){
        .ctx = ctx,
        .workers = workers,
        .workers_len = n,
    };
}

void
threadpool_enqueue(threadpool *pool, work w)
{
    work_queue_push(&pool->ctx->wq, w);
}

void
threadpool_deinit(threadpool *pool)
{
    for (ssize_t i = 0; i < pool->workers_len; ++i) {
        threadpool_enqueue(pool, (work){ 0 });
    }

    for (ssize_t i = 0; i < pool->workers_len; ++i) {
        pthread_join(pool->workers[i], NULL);
    }
    free(pool->workers);
    pool->workers = NULL;
    pool->workers_len = -1;
    printf("Counter: %d", counter);
}
