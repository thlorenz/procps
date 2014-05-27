#include <nan.h>
#include "procps.h"

using v8::Handle;
using v8::External;
using v8::Local;
using v8::Value;
using v8::ObjectTemplate;

using v8::Object;
using v8::Int32;
using v8::Uint32;
using v8::Integer;
using v8::String;
using v8::Array;

#define X(Prop, prop, convert)                    \
  NAN_GETTER(Proc::Prop) {                        \
    NanScope();                                   \
                                                  \
    Proc* self = Unwrap<Proc>(args);              \
    NanReturnValue(convert(self->_proc->prop));   \
  }

// Need special macro here since we need to down cast to uint32_t in order to avoid compiler errors
#define XUint32(Prop, prop)                                         \
  NAN_GETTER(Proc::Prop) {                                          \
    NanScope();                                                     \
                                                                    \
    Proc* self = Unwrap<Proc>(args);                                \
    NanReturnValue(NanNew<Uint32>((uint32_t) self->_proc->prop));   \
  }

// int
X(Tid, tid, NanNew<Int32>)
X(Ppid, ppid, NanNew<Int32>)
// unsigned
X(Pcpu, pcpu, NanNew<Uint32>)

// char
NAN_GETTER(Proc::State) {
  NanScope();

  Proc* self = Unwrap<Proc>(args);
  NanReturnValue(NanNew<String>(&self->_proc->state));
}

// unsigned long long
// According to http://en.wikipedia.org/wiki/C_data_types#Basic_types these are 64bit,
// however no 64bit datatype exists in JS and in order to represent them correctly 2 Int32s should be used.
// We'll cross that bridge when we get there, for now we'll assume these times stay below unit32_t max
XUint32(Utime     , utime)
XUint32(Stime     , stime)
XUint32(Cutime    , cutime)
XUint32(Cstime    , cstime)
XUint32(StartTime , start_time)

// unsigned KLONG
XUint32(StartCode  , start_code)
XUint32(EndCode    , end_code)
XUint32(StartStack , start_stack)
XUint32(KstkEsp    , kstk_esp)
XUint32(KstkEip    , kstk_eip)
XUint32(Wchan      , wchan)

#ifdef SIGNAL_STRING
//  char*
X(Signal     , signal    , NanNew<String>)
X(Blocked    , blocked   , NanNew<String>)
X(Sigignore  , sigignore , NanNew<String>)
X(Sigcatch   , sigcatch  , NanNew<String>)
X(_Sigpnd    , _sigpnd   , NanNew<String>)
#else
// long long
X(Signal     , signal    , NanNew<Integer>)
X(Blocked    , blocked   , NanNew<Integer>)
X(Sigignore  , sigignore , NanNew<Integer>)
X(Sigcatch   , sigcatch  , NanNew<Integer>)
X(_Sigpnd    , _sigpnd   , NanNew<Integer>)
#endif


// long
X(Priority , priority , NanNew<Integer>)
X(Nice     , nice     , NanNew<Integer>)
X(Rss      , rss      , NanNew<Integer>)
X(Alarm    , alarm    , NanNew<Integer>)
X(Size     , size     , NanNew<Integer>)
X(Resident , resident , NanNew<Integer>)
X(Share    , share    , NanNew<Integer>)
X(Trs      , trs      , NanNew<Integer>)
X(Lrs      , lrs      , NanNew<Integer>)
X(Drs      , drs      , NanNew<Integer>)
X(Dt       , dt       , NanNew<Integer>)

// unsigned long
XUint32(VmSize  , vm_size)
XUint32(VmLock  , vm_lock)
XUint32(VmRss   , vm_rss)
XUint32(VmData  , vm_data)
XUint32(VmStack , vm_stack)
XUint32(VmExe   , vm_exe)
XUint32(VmLib   , vm_lib)

// unsigned long
XUint32(Rtprio  , rtprio)
XUint32(Sched   , sched)
XUint32(Vsize   , vsize)
XUint32(RssRlim , rss_rlim)
XUint32(Flags   , flags)
XUint32(MinFlt  , min_flt)
XUint32(MajFlt  , maj_flt)
XUint32(CminFlt , cmin_flt)
XUint32(CmajFlt , cmaj_flt)

// char*
X(Euser  , euser  , NanNew<String>)
X(Ruser  , ruser  , NanNew<String>)
X(Suser  , suser  , NanNew<String>)
X(Fuser  , fuser  , NanNew<String>)
X(Rgroup , rgroup , NanNew<String>)
X(Egroup , egroup , NanNew<String>)
X(Sgroup , sgroup , NanNew<String>)
X(Fgroup , fgroup , NanNew<String>)
X(Cmd    , cmd    , NanNew<String>)

// *ring and *next which each point to another proc_t seem to not used, so omitted for now

// int
X(Pgrp    , pgrp    , NanNew<Int32>)
X(Session , session , NanNew<Int32>)
X(Nlwp    , nlwp    , NanNew<Int32>)
X(Tgid    , tgid    , NanNew<Int32>)
X(Tty     , tty     , NanNew<Int32>)

X(Euid, euid, NanNew<Int32>)
X(Egid, egid, NanNew<Int32>)
X(Ruid, ruid, NanNew<Int32>)
X(Rgid, rgid, NanNew<Int32>)
X(Suid, suid, NanNew<Int32>)
X(Sgid, sgid, NanNew<Int32>)
X(Fuid, fuid, NanNew<Int32>)
X(Fgid, fgid, NanNew<Int32>)

X(Tpgid      , tpgid       , NanNew<Int32>)
X(ExitSignal , exit_signal , NanNew<Int32>)
X(Processor  , processor   , NanNew<Int32>)

#undef X
#undef XUint32

// string arrays cmdline and environ
#define X(Prop, prop)                                 \
  NAN_GETTER(Proc::Prop) {                            \
    NanScope();                                       \
                                                      \
    Proc* self= Unwrap<Proc>(args);                   \
    proc_t *_proc = self->_proc;                      \
                                                      \
    Local<Array> arr;                                 \
                                                      \
    if (_proc->prop) {                                \
      int len = 0;                                    \
      while (_proc->prop[len]) len++;                 \
                                                      \
      arr = NanNew<Array>(len);                       \
                                                      \
      int i = 0;                                      \
      for (i = 0; i < len; i++) {                     \
        arr->Set(i, NanNew<String>(_proc->prop[i]));  \
      }                                               \
    } else {                                          \
      arr = NanNew<Array>(0);                         \
    }                                                 \
                                                      \
    NanReturnValue(arr);                              \
  }

X(Cmdline, cmdline)
X(Environ, environ)
#undef X

Local<Value> Proc::Wrap() {

  NanEscapableScope();

  Handle<ObjectTemplate> t = ObjectTemplate::New();
  t->SetInternalFieldCount(1);

#define X(Prop, prop) \
  t->SetAccessor(NanNew<String>(#prop), Proc::Prop);

  X(Tid        , tid)
  X(Ppid       , ppid)
  X(Pcpu       , pcpu)
  X(State      , state)

  X(Utime      , utime)
  X(Stime      , stime)
  X(Cutime     , cutime)
  X(Cstime     , cstime)
  X(StartTime  , startTime)

  X(Signal     , signal)
  X(Blocked    , blocked)
  X(Sigignore  , sigignore)
  X(Sigcatch   , sigcatch)
  X(_Sigpnd    , _sigpnd)


  X(StartCode  , startCode)
  X(EndCode    , endCode)
  X(StartStack , startStack)
  X(KstkEsp    , kstkEsp)
  X(KstkEip    , kstkEip)
  X(Wchan      , wchan)

  X(Priority   , priority)
  X(Nice       , nice)
  X(Rss        , rss)
  X(Alarm      , alarm)
  X(Size       , size)
  X(Resident   , resident)
  X(Share      , share)
  X(Trs        , trs)
  X(Lrs        , lrs)
  X(Drs        , drs)
  X(Dt         , dt)

  X(VmSize     , vmSize)
  X(VmLock     , vmLock)
  X(VmRss      , vmRss)
  X(VmData     , vmData)
  X(VmStack    , vmStack)
  X(VmExe      , vmExe)
  X(VmLib      , vmLib)

  X(Rtprio     , rtprio)
  X(Sched      , sched)
  X(Vsize      , vsize)
  X(RssRlim    , rssRlim)
  X(Flags      , flags)
  X(MinFlt     , minFlt)
  X(MajFlt     , majFlt)
  X(CminFlt    , cminFlt)
  X(CmajFlt    , cmajFlt)

  X(Cmdline    , cmdline)
  X(Environ    , environ)

  X(Euser      , euser)
  X(Ruser      , ruser)
  X(Suser      , suser)
  X(Fuser      , fuser)
  X(Rgroup     , rgroup)
  X(Egroup     , egroup)
  X(Sgroup     , sgroup)
  X(Fgroup     , fgroup)
  X(Cmd        , cmd)

  X(Pgrp       , pgrp)
  X(Session    , session)
  X(Nlwp       , nlwp)
  X(Tgid       , tgid)
  X(Tty        , tty)

  X(Euid       , euid)
  X(Egid       , egid)
  X(Ruid       , ruid)
  X(Rgid       , rgid)
  X(Suid       , suid)
  X(Sgid       , sgid)
  X(Fuid       , fuid)
  X(Fgid       , fgid)

  X(Tpgid      , tpgid)
  X(ExitSignal , exitSignal)
  X(Processor  , processor)

#undef X

  Local<Object> instance = t->NewInstance();
  instance->SetInternalField(0, NanNew<External>(this));

  return NanEscapeScope(instance);
}
