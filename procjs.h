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

#endif
