
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "v8.h"
#include "proc/readproc.h"

static void print_proctab(int flags) {
  proc_t **tab = readproctab(flags);
}

int main(void) {
  int count;
  int flags = 0;
  flags = flags | PROC_FILLCOM | PROC_FILLSTATUS | PROC_FILLSTAT | PROC_FILLMEM | PROC_FILLGRP | PROC_FILLUSR;
  print_proctab(flags);
  return 0;
}
