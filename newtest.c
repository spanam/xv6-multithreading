#include "types.h"
#include "user.h"
#include "xlib.c"

#undef NULL
#define NULL ((void*)0)


int ppid;
int global = 0;
unsigned int size = 0;
lock_t lock, lock2;
int num_threads = 30;


#define assert(x) if (x) {} else { \
   printf(1, "%s: %d ", __FILE__, __LINE__); \
   printf(1, "assert failed (%s)\n", # x); \
   printf(1, "TEST FAILED\n"); \
   kill(ppid); \
   exit(); \
}

void worker(void *arg_ptr);

int
main(int argc, char *argv[])
{
   ppid = getpid();

   int arg = 101;
   void *arg_ptr = &arg;

   xthread_lock_init(&lock);
   xthread_lock_init(&lock2);
   xthread_lock_get(&lock);
   xthread_lock_get(&lock2);

   int i;
   for (i = 0; i < num_threads; i++) {
      int thread_pid = xthread_create(worker, arg_ptr);
      assert(thread_pid > 0);
   }

   size = (unsigned int)sbrk(0);

   while (global < num_threads) {
      xthread_lock_release(&lock);
      sleep(100);
      xthread_lock_get(&lock);
   }

   global = 0;
   sbrk(10000);
   size = (unsigned int)sbrk(0);
   xthread_lock_release(&lock);

   while (global < num_threads) {
      xthread_lock_release(&lock2);
      sleep(100);
      xthread_lock_get(&lock2);
   }
   xthread_lock_release(&lock2);


   for (i = 0; i < num_threads; i++) {
      int join_pid = xthread_join();
      assert(join_pid > 0);
   }

   printf(1, "TEST PASSED\n");
   exit();
}

void
worker(void *arg_ptr) {
   xthread_lock_get(&lock);
   printf(1, "%d %d\n", size, (uint) sbrk(0));
   assert((unsigned int)sbrk(0) == size);
   global++;
   xthread_lock_release(&lock);

   xthread_lock_get(&lock2);
   assert((unsigned int)sbrk(0) == size);
   global++;
   xthread_lock_release(&lock2);

   exit();
}

