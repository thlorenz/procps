#include "procjs.h"

#define X(Prop, prop, convert)                                               \
  v8::Handle<v8::Value> Proc::Prop(                                          \
      v8::Local<v8::String> property,                                        \
      const v8::AccessorInfo& info) {                                        \
    using namespace v8;                                                      \
    HandleScope handle_scope;                                                \
                                                                             \
    Proc* self = Unwrap<Proc>(info);                                         \
    return handle_scope.Close(convert(self->_proc->prop));                   \
  }

// int
X(Tid, tid, Int32::New)
X(Ppid, ppid, Int32::New)
// unsigned
X(Pcpu, pcpu, Int32::NewFromUnsigned)
// char
v8::Handle<v8::Value> Proc::State(
    v8::Local<v8::String> property,
    const v8::AccessorInfo& info) {
  using namespace v8;
  HandleScope handle_scope;

  Proc* self = Unwrap<Proc>(info);
  return handle_scope.Close(String::New(&self->_proc->state));
}

// unsigned long long
X(Utime, utime, Uint32::New)
X(Stime, stime, Uint32::New)
X(Cutime, cutime, Uint32::New)
X(Cstime, cstime,	Uint32::New)
X(StartTime, start_time, Uint32::New)

#ifdef SIGNAL_STRING
//  char*
X(Signal, signal, String::New)
X(Blocked, blocked, String::New)
X(Sigignore, sigignore, String::New)
X(Sigcatch, sigcatch, String::New)
X(_Sigpnd, _sigpnd, String::New)
#else
// long long
X(Signal, signal, Uint32::New)
X(Blocked, blocked, Uint32::New)
X(Sigignore, sigignore, Uint32::New)
X(Sigcatch, sigcatch, Uint32::New)
X(_Sigpnd, _sigpnd, Uint32::New)
#endif

// unsigned KLONG
X(StartCode, start_code, Uint32::New)
X(EndCode, end_code, Uint32::New)
X(StartStack, start_stack, Uint32::New)
X(KstkEsp, kstk_esp, Uint32::New)
X(KstkEip, kstk_eip, Uint32::New)
X(Wchan, wchan, Uint32::New)

// long
X(Priority, priority, Uint32::New)
X(Nice, nice, Uint32::New)
X(Rss, rss, Uint32::New)
X(Alarm, alarm, Uint32::New)
X(Size, size, Uint32::New)
X(Resident, resident, Uint32::New)
X(Share, share, Uint32::New)
X(Trs, trs, Uint32::New)
X(Lrs, lrs, Uint32::New)
X(Drs, drs, Uint32::New)
X(Dt, dt, Uint32::New)

// unsigned long
X(VmSize, vm_size, Uint32::New)
X(VmLock, vm_lock, Uint32::New)
X(VmRss, vm_rss, Uint32::New)
X(VmData, vm_data, Uint32::New)
X(VmStack, vm_stack, Uint32::New)
X(VmExe, vm_exe, Uint32::New)
X(VmLib, vm_lib, Uint32::New)

// unsigned long
X(Rtprio, rtprio, Uint32::New)
X(Sched, sched, Uint32::New)
X(Vsize, vsize, Uint32::New)
X(RssRlim, rss_rlim, Uint32::New)
X(Flags, flags, Uint32::New)
X(MinFlt, min_flt, Uint32::New)
X(MajFlt, maj_flt, Uint32::New)
X(CminFlt, cmin_flt, Uint32::New)
X(CmajFlt, cmaj_flt, Uint32::New)

// char*
X(Euser, euser, String::New)
X(Ruser, ruser, String::New)
X(Suser, suser, String::New)
X(Fuser, fuser, String::New)
X(Rgroup, rgroup, String::New)
X(Egroup, egroup, String::New)
X(Sgroup, sgroup, String::New)
X(Fgroup, fgroup, String::New)
X(Cmd, cmd, String::New)

// *ring and *next which each point to another proc_t seem to not used, so omitted for now

// int
X(Pgrp, pgrp, Uint32::New)
X(Session, session, Uint32::New)
X(Nlwp, nlwp, Uint32::New)
X(Tgid, tgid, Uint32::New)
X(Tty, tty, Uint32::New)

X(Euid, euid, Uint32::New)
X(Egid, egid, Uint32::New)
X(Ruid, ruid, Uint32::New)
X(Rgid, rgid, Uint32::New)
X(Suid, suid, Uint32::New)
X(Sgid, sgid, Uint32::New)
X(Fuid, fuid, Uint32::New)
X(Fgid, fgid, Uint32::New)

X(Tpgid, tpgid, Uint32::New)
X(ExitSignal, exit_signal, Uint32::New)
X(Processor, processor, Uint32::New)

#undef X

// string arrays cmdline and environ
#define X(Prop, prop)                                                       \
  v8::Handle<v8::Value> Proc::Prop(                                         \
      v8::Local<v8::String> property,                                       \
      const v8::AccessorInfo& info) {                                       \
    using namespace v8;                                                     \
    HandleScope handle_scope;                                               \
                                                                            \
    Proc* self= Unwrap<Proc>(info);                                         \
    proc_t *_proc = self->_proc;                                            \
                                                                            \
    Local<Array> arr;                                                       \
                                                                            \
    if (_proc->prop) {                                                      \
      int len = 0;                                                          \
      while (_proc->prop[len]) len++;                                       \
                                                                            \
      arr = Array::New(len);                                                \
                                                                            \
      int i = 0;                                                            \
      for (i = 0; i < len; i++) {                                           \
        arr->Set(i, String::New(_proc->prop[i]));                           \
      }                                                                     \
    } else {                                                                \
      arr = Array::New(0);                                                  \
    }                                                                       \
                                                                            \
    return handle_scope.Close(arr);                                         \
  }

X(Cmdline, cmdline)
X(Environ, environ)
#undef X

v8::Local<v8::Value> Proc::Wrap() {
  using namespace v8;

  HandleScope handle_scope;
  Handle<ObjectTemplate> t = ObjectTemplate::New();
  t->SetInternalFieldCount(1);

#define X(Prop, prop) \
  t->SetAccessor(String::New(#prop), Proc::Prop);

  X(Tid, tid)
  X(Ppid, ppid)
  X(Pcpu, pcpu)
  X(State, state)

  X(Utime, utime)
  X(Stime, stime)
  X(Cutime, cutime)
  X(Cstime, cstime)
  X(StartTime, startTime)

  X(Signal, signal)
  X(Blocked, blocked)
  X(Sigignore, sigignore)
  X(Sigcatch, sigcatch)
  X(_Sigpnd, _sigpnd)


	X(StartCode, startCode)
	X(EndCode, endCode)
	X(StartStack, startStack)
	X(KstkEsp, kstkEsp)
	X(KstkEip, kstkEip)
	X(Wchan, wchan)

  X(Priority, priority)
  X(Nice, nice)
  X(Rss, rss)
  X(Alarm, alarm)
  X(Size, size)
  X(Resident, resident)
  X(Share, share)
  X(Trs, trs)
  X(Lrs, lrs)
  X(Drs, drs)
  X(Dt, dt)

  X(VmSize, vmSize)
  X(VmLock, vmLock)
  X(VmRss, vmRss)
  X(VmData, vmData)
  X(VmStack, vmStack)
  X(VmExe, vmExe)
  X(VmLib, vmLib)

  X(Rtprio, rtprio)
  X(Sched, sched)
  X(Vsize, vsize)
  X(RssRlim, rssRlim)
  X(Flags, flags)
  X(MinFlt, minFlt)
  X(MajFlt, majFlt)
  X(CminFlt, cminFlt)
  X(CmajFlt, cmajFlt)

  X(Cmdline, cmdline)
  X(Environ, environ)

  X(Euser, euser)
  X(Ruser, ruser)
  X(Suser, suser)
  X(Fuser, fuser)
  X(Rgroup, rgroup)
  X(Egroup, egroup)
  X(Sgroup, sgroup)
  X(Fgroup, fgroup)
  X(Cmd, cmd)

  X(Pgrp, pgrp)
  X(Session, session)
  X(Nlwp, nlwp)
  X(Tgid, tgid)
  X(Tty, tty)

  X(Euid, euid)
  X(Egid, egid)
  X(Ruid, ruid)
  X(Rgid, rgid)
  X(Suid, suid)
  X(Sgid, sgid)
  X(Fuid, fuid)
  X(Fgid, fgid)

  X(Tpgid, tpgid)
  X(ExitSignal, exitSignal)
  X(Processor, processor)

#undef X

  Local<Object> instance = t->NewInstance();
  instance->SetInternalField(0, External::New(this));

  return handle_scope.Close(instance);
}
