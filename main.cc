#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "v8.h"
#include "proc/readproc.h"

using namespace std;

static void print_proctab(int flags) {
  proc_t **tab = readproctab(flags);
}

static proc_t **get_proctab() {
  int flags = 0;
  flags = flags | PROC_FILLCOM | PROC_FILLSTATUS | PROC_FILLSTAT | PROC_FILLMEM | PROC_FILLGRP | PROC_FILLUSR;
  return readproctab(flags);
}

v8::Handle<v8::String> GetScript(v8::Isolate* isolate);

void AddFunction(v8::Isolate* isolate, v8::Handle<v8::Object> global, const char* name, v8::FunctionCallback callback) {
  using namespace v8;
  HandleScope handle_scope(isolate);
  global->Set(String::NewFromUtf8(isolate, name), FunctionTemplate::New(isolate, callback)->GetFunction());
}

class Procjs {
  proc_t **_pt;
  unsigned int _len;
public:
  Procjs() {
    refresh();
  }

  void refresh() {
    _pt = get_proctab();
    _len = -1;

    while(*(_pt + (++_len)));
  }

  inline proc_t *proctab() { return _pt[0]; };
  unsigned int length() { return _len; }
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
  info.GetReturnValue().Set(Number::New(isolate, pjs->length()));
}

v8::Handle<v8::String> GetScript(v8::Isolate* isolate) {
  string src =
    "'use strict';"
    ""
    "var pjs = new Procjs();"
    "(function () { return pjs.length; })();"
    ;

  const char *js = src.c_str();
  return v8::String::NewFromUtf8(isolate, js);
}

void ProcjsCtor(const v8::FunctionCallbackInfo<v8::Value>& info) {
  using namespace v8;
  Isolate *isolate = info.GetIsolate();
  HandleScope handle_scope(isolate);

  Handle<ObjectTemplate> t = ObjectTemplate::New();
  t->SetInternalFieldCount(1);
  t->SetAccessor(String::NewFromUtf8(isolate, "length"), GetProcjsLength);

  Procjs *pjs = new Procjs();
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

  Handle<String> src = GetScript(isolate);
  Handle<Value> result = Script::Compile(src)->Run();

  cout << "Result: " << *String::Utf8Value(result);
  return 0;
}

