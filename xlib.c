#include "stat.h"
#include "types.h"
#include "x86.h"
#include "fcntl.h"
#include "user.h"

#define PGSIZE 4096

int xthread_create(void (*func)(void*), void *args) {
  void *stack = malloc(PGSIZE*2);
  if(!stack) {
    printf(1, "Error: malloc failure\n");
    exit();
  }
  if((uint)stack % PGSIZE) 
    stack += (4096 - (uint)stack % PGSIZE);

  return clone(func, args, stack);
}

int xthread_join() {
  void *stack;
  int tid = join(&stack);
  if(tid != -1)
    free(stack);
  return 0;
}

void xthread_lock_init(volatile lock_t *lock) {
  *lock = 0;
}

void xthread_lock_get(volatile lock_t *lock) {
  while(xchg(lock, 1) == 1);
}

void xthread_lock_release(volatile lock_t *lock) {
  xchg(lock, 0);
}
