// Has 5 threads increment a number until completion,
// similar to what we did in class.

#include "types.h"
#include "xlib.c"

int counter = 0;
lock_t lock;

void runner(void *ptr) {
  int i;
  xthread_lock_get(&lock);
  for(i = 0; i < 10000; i++)
    counter++;
  xthread_lock_release(&lock);
  exit();
}

int main() {
  printf(1, "Beginning test: 5 threads each count 10000 times\n");
  for(int i = 0; i < 5; i++) {
    void *arg = 0;
    xthread_create(runner, arg);
  }
  for(int i = 0; i < 5; i++) {
    xthread_join();
  }
  if(counter == 50000)
    printf(1, "Test passed: 5 threads counted to 50000\n");
  else
    printf(1, "Test failed\n");
  exit();
}
