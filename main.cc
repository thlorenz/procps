#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//#include "v8.h"
#include "deps/v8/include/v8.h"
#include "proc/readproc.h"
#include "procjs.h"

using namespace std;

static void print_proctab(int flags) {
  proc_t **tab = readproctab(flags);
}

static proc_t **get_proctab() {
  int flags = 0;
  flags = flags | PROC_FILLCOM | PROC_FILLSTATUS | PROC_FILLSTAT | PROC_FILLMEM | PROC_FILLGRP | PROC_FILLUSR;
  return readproctab(flags);
}

void AddFunction(v8::Isolate* isolate, v8::Handle<v8::Object> global, const char* name, v8::FunctionCallback callback) {
  using namespace v8;
  HandleScope handle_scope(isolate);
  global->Set(String::NewFromUtf8(isolate, name), FunctionTemplate::New(isolate, callback)->GetFunction());
}

v8::Handle<v8::String> GetScript(v8::Isolate* isolate);
void GetProcPid(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
void GetProcCmd(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
void GetProcCmdline(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

class Proc {
  v8::Isolate *_isolate;
  proc_t *_proc;

public:
  Proc(v8::Isolate* isolate, proc_t *proc) : _isolate(isolate), _proc(proc) {}

  unsigned int Pid() const {
    return _proc->ppid;
  }

  v8::Handle<v8::Array> Cmdline() const {
    using namespace v8;
    EscapableHandleScope scope(_isolate);

    if (_proc->cmdline) {
      int len = 0;
      while (_proc->cmdline[len]) len++;

      Local<Array> arr = Array::New(_isolate, len);

      int i = 0;
      for (i = 0; i < len; i++) {
        arr->Set(Integer::New(_isolate, i), String::NewFromUtf8(_isolate, _proc->cmdline[i]));
      }

      return scope.Escape(arr);
    } else {
      return scope.Escape(Array::New(_isolate, 0));
    }
  }

  char* Cmd() const {
    return _proc->cmd;
  }

  v8::Handle<v8::Object> Wrap() {
    using namespace v8;

    // HandleScope handle_scope(_isolate);
    Handle<ObjectTemplate> t = ObjectTemplate::New(_isolate);

    t->SetInternalFieldCount(1);
    t->SetAccessor(String::NewFromUtf8(_isolate, "pid"), GetProcPid);
    t->SetAccessor(String::NewFromUtf8(_isolate, "cmd"), GetProcCmd);
    t->SetAccessor(String::NewFromUtf8(_isolate, "cmdline"), GetProcCmdline);

    Local<Object> instance = t->NewInstance();
    instance->SetInternalField(0, External::New(_isolate, this));

    return instance;
  }
};

class Procjs {
  v8::Isolate *_isolate;
  proc_t **_pt;
  unsigned int _len;

  Proc* procAt(const int idx) const {
    return new Proc(_isolate, _pt[idx]);
  };

public:
  Procjs(v8::Isolate* isolate): _isolate(isolate) {
    Refresh();
  }

  void Refresh() {
    _pt = get_proctab();
    _len = -1;

    while(*(_pt + (++_len)));
  }

  v8::Handle<v8::Array> Procs() {
    using namespace v8;
    EscapableHandleScope scope(_isolate);

    Local<Array> arr = Array::New(_isolate, _len);

    int i = 0;
    for (i = 0; i < _len; i++) {
      arr->Set(Integer::New(_isolate, i), procAt(i)->Wrap());
    }

    return scope.Escape(arr);
  }

  unsigned int Length() {
    return _len;
  }
};

template <typename T, typename CallbackInfo> T* Unwrap(const CallbackInfo& info) {
  using namespace v8;
  Isolate *isolate = info.GetIsolate();
  HandleScope handle_scope(isolate);

  Local<Object> self = info.Holder();
  Local<External> external = Local<External>::Cast(self->GetInternalField(0));

  return static_cast<T*>(external->Value());
}

void GetProcjsLength(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
  using namespace v8;
  Isolate *isolate = info.GetIsolate();
  HandleScope handle_scope(isolate);

  Procjs* pjs = Unwrap<Procjs>(info);
  info.GetReturnValue().Set(Number::New(isolate, pjs->Length()));
}

void GetProcs(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
  using namespace v8;
  Isolate *isolate = info.GetIsolate();
  HandleScope handle_scope(isolate);

  Procjs* pjs = Unwrap<Procjs>(info);
  Handle<Array> procs = pjs->Procs();

  info.GetReturnValue().Set(procs);
}

void GetProcPid(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
  using namespace v8;
  Isolate *isolate = info.GetIsolate();
  HandleScope handle_scope(isolate);

  Proc* proc= Unwrap<Proc>(info);
  info.GetReturnValue().Set(Number::New(isolate, proc->Pid()));
}

void GetProcCmd(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
  using namespace v8;
  Isolate *isolate = info.GetIsolate();
  HandleScope handle_scope(isolate);

  Proc* proc= Unwrap<Proc>(info);
  info.GetReturnValue().Set(String::NewFromUtf8(isolate, proc->Cmd()));
}

void GetProcCmdline(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
  using namespace v8;
  Isolate *isolate = info.GetIsolate();
  HandleScope handle_scope(isolate);

  Proc* proc= Unwrap<Proc>(info);
  info.GetReturnValue().Set(proc->Cmdline());
}

void ProcjsCtor(const v8::FunctionCallbackInfo<v8::Value>& info) {
  using namespace v8;
  Isolate *isolate = info.GetIsolate();
  HandleScope handle_scope(isolate);

  Handle<ObjectTemplate> t = ObjectTemplate::New();
  t->SetInternalFieldCount(1);
  t->SetAccessor(String::NewFromUtf8(isolate, "length"), GetProcjsLength);
  t->SetAccessor(String::NewFromUtf8(isolate, "procs"), GetProcs);

  Procjs *pjs = new Procjs(isolate);
  Local<Object> instance = t->NewInstance();
  instance->SetInternalField(0, External::New(isolate, pjs));

  info.GetReturnValue().Set(instance);
}


int main(void) {
  using namespace v8;
  Isolate *isolate = Isolate::GetCurrent();
  HandleScope handle_scope(isolate);
  Handle<Context> context = Context::New(isolate);
  Context::Scope context_scope(context);

  Handle<Object> global = context->Global();
  AddFunction(isolate, global, "Procjs", ProcjsCtor);

  Handle<String> src = ReadFile(isolate, "test.js");
  Handle<Value> result = Script::Compile(src)->Run();

  cout << "Result: " << endl << *String::Utf8Value(result);
  return 0;
}

// vim: ft=cpp
