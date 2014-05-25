#ifndef __PROCJS_H__
#define __PROCJS_H__

#include "v8.h"
#include "proc/readproc.h"

template <typename T, typename CallbackInfo> T* Unwrap(const CallbackInfo& info) {
  using namespace v8;
  HandleScope handle_scope;

  Local<Object> self = info.Holder();
  Local<External> external = Local<External>::Cast(self->GetInternalField(0));

  return static_cast<T*>(external->Value());
}

class Proc {
  v8::Isolate *_isolate;
  proc_t *_proc;

public:

  Proc(v8::Isolate* isolate, proc_t *proc)
    : _isolate(isolate), _proc(proc) {}

#define X(Prop) \
  static v8::Handle<v8::Value> Prop(v8::Local<v8::String> property, const v8::AccessorInfo& info);

  X(Tid)        // (special)       task id, the POSIX thread ID (see also: tgid)
  X(Ppid)       // stat,status     pid of parent process
  X(Pcpu)       // stat (special)  %CPU usage (is not filled in by readproc!!!)
  X(State)      // stat,status     single-char code for process state (S=sleeping)

  X(Utime)      // stat            user-mode CPU time accumulated by process
  X(Stime)      // stat            kernel-mode CPU time accumulated by process
  X(Cutime)     // stat            cumulative utime of process and reaped children
  X(Cstime)     // stat            cumulative stime of process and reaped children
  X(StartTime)  // stat            start time of process -- seconds since 1-1-70


  X(Signal)     // status          mask of pending signals) per-task for readtask() but per-proc for readproc()
  X(Blocked)    // status          mask of blocked signals
  X(Sigignore)  // status          mask of ignored signals
  X(Sigcatch)   // status          mask of caught  signals
  X(_Sigpnd)    // status          mask of PER TASK pending signals

  X(StartCode)  // stat            address of beginning of code segment
  X(EndCode)    // stat            address of end of code segment
  X(StartStack) // stat            address of the bottom of stack for the process
  X(KstkEsp)    // stat            kernel stack pointer
  X(KstkEip)    // stat            kernel instruction pointer
  X(Wchan)      // stat (special)  address of kernel wait channel proc is sleeping in


  X(Priority)   // stat            kernel scheduling priority
  X(Nice)       // stat            standard unix nice level of process
  X(Rss)        // stat            resident set size from /proc/#/stat (pages)
  X(Alarm)      // stat            ?
  // the next 7 members come from /proc/#/statm
  X(Size)       // statm           total # of pages of memory
  X(Resident)   // statm           number of resident set (non-swapped) pages (4k)
  X(Share)      // statm           number of pages of shared (mmap'd) memory
  X(Trs)        // statm           text resident set size
  X(Lrs)        // statm           shared-lib resident set size
  X(Drs)        // statm           data resident set size
  X(Dt)         // statm           dirty pages


  X(VmSize)     // status          same as vsize in kb
  X(VmLock)     // status          locked pages in kb
  X(VmRss)      // status          same as rss in kb
  X(VmData)     // status          data size
  X(VmStack)    // status          stack size
  X(VmExe)      // status          executable size
  X(VmLib)      // status          library size (all pages, not just used ones)


  X(Rtprio)     // stat            real-time priority
  X(Sched)      // stat            scheduling class
  X(Vsize)      // stat            number of pages of virtual memory ...
  X(RssRlim)    // stat            resident set size limit?
  X(Flags)      // stat            kernel flags for the process
  X(MinFlt)     // stat            number of minor page faults since process start
  X(MajFlt)     // stat            number of major page faults since process start
  X(CminFlt)    // stat            cumulative min_flt of process and child processes
  X(CmajFlt)    // stat            cumulative maj_flt of process and child processes

  X(Euser)      // stat(),status   effective user name
  X(Ruser)      // status          real user name
  X(Suser)      // status          saved user name
  X(Fuser)      // status          filesystem user name
  X(Rgroup)     // status          real group name
  X(Egroup)     // status          effective group name
  X(Sgroup)     // status          saved group name
  X(Fgroup)     // status          filesystem group name
  X(Cmd)        // stat,status     basename of executable file in call to exec(2)

  X(Pgrp)       // stat            process group id
  X(Session)    // stat            session id
  X(Nlwp)       // stat,status     number of threads, or 0 if no clue
  X(Tgid)       // (special)       task group ID, the POSIX PID (see also: tid)
  X(Tty)        // stat            full device number of controlling terminal

  X(Euid)
  X(Egid)       // stat(),status   effective
  X(Ruid)
  X(Rgid)       // status          real
  X(Suid)
  X(Sgid)       // status          saved
  X(Fuid)
  X(Fgid)       // status          fs (used for file access only)

  X(Tpgid)      // stat            terminal process group id
  X(ExitSignal) // stat            might not be SIGCHLD
  X(Processor)  // stat            current (or most recent?) CPU

  X(Environ)    // (special)       environment string vector (/proc/#/environ)
  X(Cmdline)    // (special)       command line string vector (/proc/#/cmdline)

#undef X

  v8::Local<v8::Value> Wrap();
};

#endif
