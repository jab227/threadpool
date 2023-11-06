#include "pool.h"
#include <stdio.h>

#define UNUSED(x) ((void)x)

void print1(void *arg) {
  UNUSED(arg);
  puts("writing from 1");
}

void print2(void *arg) {
  UNUSED(arg);
  puts("writing from 2");
}

void print3(void *arg) {
  UNUSED(arg);
  puts("writing from 3");
}

void print4(void *arg) {
  UNUSED(arg);
  puts("writing from 4");
}

void print5(void *arg) {
  UNUSED(arg);
  puts("writing from 5");
}

void print6(void *arg) {
  UNUSED(arg);
  puts("writing from 6");
}

void print7(void *arg) {
  UNUSED(arg);
  puts("writing from 7");
}

void print8(void *arg) {
  UNUSED(arg);
  puts("writing from 8");
}

static work_cb wcb[8] = {print1, print2, print3, print4,
                         print5, print6, print7, print8};

int main(void) {
  work_queue wq = work_queue_init();
  struct context ctx = {.wq = wq};
  threadpool pool = threadpool_init(&ctx, 6);
  ssize_t i = 0;
  while (i < 800) {
    threadpool_enqueue(&pool, (work){.cb = wcb[i & 7]});
    ++i;
  }

  threadpool_deinit(&pool);
  work_queue_deinit(&wq);
  return 0;
}
