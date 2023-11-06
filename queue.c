#include "queue.h"

work_queue work_queue_init() {
  pthread_mutex_t buf_lock;
  pthread_cond_t not_empty, not_full;

  pthread_mutex_init(&buf_lock, NULL);
  pthread_cond_init(&not_full, NULL);
  pthread_cond_init(&not_empty, NULL);
  return (work_queue){
      .buf_lock = buf_lock,
      .not_full = not_full,
      .not_empty = not_empty,
  };
}

void work_queue_deinit(work_queue *wq) {
  pthread_mutex_destroy(&wq->buf_lock);
  pthread_cond_destroy(&wq->not_full);
  pthread_cond_destroy(&wq->not_empty);
}

void work_queue_push(work_queue *q, work w) {
  pthread_mutex_lock(&q->buf_lock);
  while (((q->head + 1) & (QUEUE_SIZE - 1)) == q->tail) {
    pthread_cond_wait(&q->not_full, &q->buf_lock);
  }
  q->buf[q->head] = w;
  q->head = (q->head + 1) & (QUEUE_SIZE - 1);
  assert(q->head < QUEUE_SIZE);
  pthread_cond_broadcast(&q->not_empty);
  pthread_mutex_unlock(&q->buf_lock);
}

work work_queue_pop(work_queue *q) {
  pthread_mutex_lock(&q->buf_lock);
  while (q->head == q->tail) {
    pthread_cond_wait(&q->not_empty, &q->buf_lock);
  }
  work w = q->buf[q->tail];
  q->tail = (q->tail + 1) & (QUEUE_SIZE - 1);
  assert(q->tail < QUEUE_SIZE);
  pthread_cond_broadcast(&q->not_full);
  pthread_mutex_unlock(&q->buf_lock);
  return w;
}
