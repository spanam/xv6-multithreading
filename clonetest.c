// Test that clone fails gracefully.
// Tiny executable so that the limit can be filling the proc table.

#include "types.h"
#include "xlib.c"

int i = 0;

void runner(void *ptr) {
  i++;
  char* msg = *(char**)ptr;
  printf(1, msg);
  exit();
}

int
main(void)
{
  char *msg = "[Child]: successfully received message from parent\n";
  void *arg = &msg;
  xthread_create(runner, arg);
  sleep(10);
  xthread_join();
  if(i)
    printf(1, "[Parent]: successful modification of shared variable by child\n");
  else
    printf(1, "[Parent]: unsuccessful modification of shared variable by child\n");
  exit();
}
