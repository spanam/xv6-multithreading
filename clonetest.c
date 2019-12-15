// Test that clone fails gracefully.
// Tiny executable so that the limit can be filling the proc table.

#include "types.h"
#include "xlib.c"


int i = 0;

void runner(void* j)
{
  printf(1, "child running\n");
  i++;
}

int
main(void)
{
  void *j = 0;
  xthread_create(runner, j);
  xthread_join();
  if(i)
    printf(1, "child thread successful\n");
  else
    printf(1, "child thread needs work\n");
  exit();
}
