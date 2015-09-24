#define _GNU_SOURCE
#include <numa.h>
#include <sched.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COUNT 1024*1024*128

#define SMT0 0
#define SMT1 4

#define CORE0 0
#define CORE1 1

volatile int sum;

struct timespec start, end;

pthread_t t0, t1;

void *thread_function(void *V) {
  int i;
  for (i = 0; i < COUNT; ++i)
    __sync_add_and_fetch(&sum, 1);

  pthread_exit(NULL);
}

int run(int c0, int c1) {
  cpu_set_t cpu0, cpu1;
  CPU_ZERO(&cpu0);
  CPU_ZERO(&cpu1);

  CPU_SET(c0,&cpu0);
  CPU_SET(c1,&cpu1);

  int ret;
  ret = pthread_create(&t0, NULL, thread_function, NULL);
  ret |= pthread_create(&t1, NULL, thread_function, NULL);

  ret |= pthread_setaffinity_np(t0, sizeof(cpu_set_t), &cpu0);
  ret |= pthread_setaffinity_np(t1, sizeof(cpu_set_t), &cpu1);

  if (ret) {
    fprintf(stderr, "Thread creation failed.\n");
    exit(EXIT_FAILURE);
  }

  pthread_join(t0, NULL);
  pthread_join(t1, NULL);

  return 0;
}

int bench(int c0, int c1) {
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);

  run(c0, c1);

  clock_gettime(CLOCK_MONOTONIC_RAW, &end);

  float delta = ((float) end.tv_sec + end.tv_nsec*1e-9) - ((float) start.tv_sec+start.tv_nsec*1e-9);
  printf("sum: %d, time: %f\n", sum, delta);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s [0|1]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (atoi(argv[1])) {
    printf("smt: ");
    bench(SMT0,SMT1);
  } else {
    printf("core: ");
    bench(CORE0,CORE1);
  }

  exit(EXIT_SUCCESS);
}
