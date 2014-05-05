#ifndef __PROCJS_H__
#define __PROCJS_H__

#include "v8.h"
#include "proc/readproc.h"

template <typename T, typename CallbackInfo> T* Unwrap(const CallbackInfo& info) {
  using namespace v8;
  Isolate *isolate = info.GetIsolate();
  HandleScope handle_scope(isolate);

  Local<Object> self = info.Holder();
  Local<External> external = Local<External>::Cast(self->GetInternalField(0));

  return static_cast<T*>(external->Value());
}

v8::Handle<v8::String> ReadFile(v8::Isolate* isolate, const char* name);

class Proc {
  v8::Isolate *_isolate;
  proc_t *_proc;

public:

  Proc(v8::Isolate* isolate, proc_t *proc)
    : _isolate(isolate), _proc(proc) {}

  static void Pid(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
  static void Cmd(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
  static void Cmdline(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

  v8::Handle<v8::Object> Wrap();
};

class Procjs {
  v8::Isolate *_isolate;
  proc_t **_pt;
  unsigned int _len;

  static proc_t **get_proctab() {
    int flags = 0;
    flags = flags | PROC_FILLCOM | PROC_FILLSTATUS | PROC_FILLSTAT | PROC_FILLMEM | PROC_FILLGRP | PROC_FILLUSR;
    return readproctab(flags);
  }


  Proc* procAt(const int idx) const {
    return new Proc(_isolate, _pt[idx]);
  };

  void refresh() {
    _pt = get_proctab();
    _len = -1;

    while(*(_pt + (++_len)));
  }

public:
  Procjs(v8::Isolate* isolate): _isolate(isolate) {
    refresh();
  }

  static void New(const v8::FunctionCallbackInfo<v8::Value>& info);
  static void Procs(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
  static void Refresh(const v8::FunctionCallbackInfo<v8::Value>& info);
};
#endif
