#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "proc/readproc.h"

static void print_row(proc_t* tsk) {

	char cmd[4096];
  int opt_full = 1;

  if (opt_full && tsk->cmdline) {
    int i = 0;
    int bytes = sizeof (cmd) - 1;

    /* make sure it is always NUL-terminated */
    cmd[bytes] = 0;
    /* make room for SPC in loop below */
    --bytes;

    strncpy (cmd, tsk->cmdline[i], bytes);
    bytes -= strlen (tsk->cmdline[i++]);
    while (tsk->cmdline[i] && bytes > 0) {
      strncat (cmd, " ", bytes);
      strncat (cmd, tsk->cmdline[i], bytes);
      bytes -= strlen (tsk->cmdline[i++]) + 1;
    }
  } else {
    strcpy (cmd, tsk->cmd);
  }

  printf("PID: %d \tUSER: %s \tNI: %ld, VIRT: %ld \tRES: %ld \tSHR: %ld \tS: %c \tTIME: %lld \tCMD: %s \n",
        tsk->ppid, tsk->euser, tsk->nice, tsk->vm_size, tsk->resident, tsk->share, tsk->state,
        tsk->utime + tsk->stime, cmd);

}

static void print_proctab(int flags) {
  proc_t **tab = readproctab(flags);
  proc_t* row;
  while((row = *(tab++))) {
    print_row(row);
  }
}

int main(void) {
  int count;
  int flags = 0;
  flags = flags | PROC_FILLCOM | PROC_FILLSTATUS | PROC_FILLSTAT | PROC_FILLMEM | PROC_FILLGRP | PROC_FILLUSR;
  print_proctab(flags);
  return 0;
}
