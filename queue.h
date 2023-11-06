#ifndef QUEUE_H_
#define QUEUE_H_

#include <assert.h>
#include <pthread.h>
#include <sys/types.h>

#ifndef QUEUE_SIZE
#define QUEUE_SIZE 8
#endif

static_assert(((QUEUE_SIZE & (QUEUE_SIZE - 1)) == 0), "should be power of 2");

typedef void (*work_cb)(void *data);
typedef struct {
  work_cb cb;
  void *data;
} work;

// Size defined at compile time, see QUEUE_SIZE. Must be power of
// two. The capacity of the queue es QUEUE_SIZE - 1
typedef struct {
  ssize_t head;
  ssize_t tail;
  pthread_mutex_t buf_lock;
  pthread_cond_t not_empty;
  pthread_cond_t not_full;
  work buf[QUEUE_SIZE];
} work_queue;


work_queue work_queue_init();
void work_queue_deinit(work_queue *wq);
void work_queue_push(work_queue *q, work w);
work work_queue_pop(work_queue *q);

#endif
