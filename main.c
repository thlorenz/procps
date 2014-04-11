#include <stdio.h>
#include "proc/readproc.h"

int main(void) {
  PROCTAB* ptab = openproc(PROC_PID);
  fprintf(stderr, "ptab path: %s", ptab->path);

  return 0;
}
