#define _REENTRANT
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sched.h>
#include "sthread.h"

#define NUMTHR 10
#define NUMLOOP 25

int balance;
sthread_rwlock_t mylock;

int threadmain(void *arg)
{
  int threadno = (int)arg;
  for (;;) {
    printf("thread %d: I'm going to sleep\n", threadno);
    sthread_suspend();
    printf("thread %d: I woke up!\n", threadno);
  }
  return 0;
}

int slowreader(void *arg) {
  int threadno = (int)arg;
  sthread_read_lock(&mylock);
  printf("thread%d ===> This reader thread is very slow\n", threadno);
  sleep(10);
  sthread_read_unlock(&mylock);
  printf("thread%d ===> I am finally done! #############\n", threadno);
  return 0;
}

int bcheck(void *arg) {
  int i;
  int trubal;
  int threadno = (int)arg;
  for (i = 0; i<5; i++) {
    sthread_read_lock(&mylock);
    trubal = balance;
    printf("thread%d ===> Reader thread iteration %d, balance = %d\n", threadno, i, trubal);
    fflush(stdout);
    sthread_read_unlock(&mylock);
    //sleep(1);
  }
  return 0;
}

int deposit(void *arg) {
  int i, tmp;
  //k = 0;
  int threadno = (int)arg;
  for (i=0; i<NUMLOOP; i++) {
    sthread_write_lock(&mylock);
    tmp = balance;
    tmp = tmp + 1;
    balance = tmp;
    sthread_write_unlock(&mylock);
    printf("thread%d ===> Deposit thread iteration %d\n", threadno, i);
    fflush(stdout);
  }
  return 0;
}

int withdraw(void *arg) {
  int i;
  int threadno = (int)arg;
  for (i=0; i<NUMLOOP; i++) {
    sthread_write_lock(&mylock);
    balance -= 1;
    sthread_write_unlock(&mylock);
    printf("thread%d ===> Withdrawal thread iteration %d\n", threadno, i);
    fflush(stdout);
  }
  return 0;
}

int main(int argc, char *argv[])
{
  sthread_t thr1[NUMTHR], thr2[NUMTHR], thr3[NUMTHR];
  //sthread_t thr0;
  int i;
  balance = 10000;
  int destroyed = 0;

  sthread_rwlock_init(&mylock);

  if (sthread_init() == -1)
    fprintf(stderr, "%s: sthread_init: %s\n", argv[0], strerror(errno));


  for (i = 0; i<NUMTHR; i++) {
    if (sthread_create(&thr3[i], bcheck, (void *)(NUMTHR+NUMTHR+i)) == -1)
      fprintf(stderr, "%s: sthread_create: %s\n", argv[0], strerror(errno));

    if (sthread_create(&thr1[i], deposit, (void *)(1+i)) == -1)
      fprintf(stderr, "%s: sthread_create: %s\n", argv[0], strerror(errno));

    if (sthread_create(&thr2[i], withdraw, (void *)(NUMTHR+i)) == -1)
      fprintf(stderr, "%s: sthread_create: %s\n", argv[0], strerror(errno));
  }

  sleep(1);

  while (1) {
    //sleep(1);
    //sthread_wake(thr1);
    //sthread_wake(thr2);
    sleep(3);
    printf("the balnce is %d\n", balance);
    if (destroyed == 0) {
      sthread_rwlock_destroy(&mylock);
      destroyed = 1;
    }
  }

  return 0;
}

