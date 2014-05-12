#include "procjs.h"
#include <cassert>

#define MAX_PROCS 5000

typedef v8::Handle<v8::Value> (*FunctionCallback)(const v8::Arguments& info);
void AddFunction(v8::Handle<v8::Object> global, const char* name, FunctionCallback callback) {
  global->Set(v8::String::New(name), v8::FunctionTemplate::New(callback)->GetFunction());
}

v8::Handle<v8::Value> Procjs::New(const v8::Arguments& args) {
  using namespace v8;
  Isolate *isolate = args.GetIsolate();
  HandleScope handle_scope;

  // our JS API ensures we always pass flags (defaults if none were given)
  Local<Integer> flags = args[0].As<Integer>();
  assert(flags->IsUint32());

  Handle<ObjectTemplate> t = ObjectTemplate::New();
  t->SetInternalFieldCount(1);

  Procjs *self = new Procjs(isolate, flags->Uint32Value());
  Local<Object> instance = t->NewInstance();
  instance->SetInternalField(0, External::New(self));

  AddFunction(instance, "procs", Procjs::Procs);
  AddFunction(instance, "refresh", Procjs::Refresh);

  return handle_scope.Close(instance);
}

v8::Handle<v8::Value> Procjs::Procs(const v8::Arguments& args) {
  using namespace v8;
  Isolate *isolate = args.GetIsolate();
  HandleScope handle_scope;

  Local<Function> cb = args[0].As<Function>();
  assert(cb->IsFunction());

  Procjs* self = Unwrap<Procjs>(args);
  assert(self->_len <= MAX_PROCS && "exceeded MAX_PROCS");

  Local<Value> argv[MAX_PROCS];
  for (int i = 0; i < self->_len; i++) {
    argv[i] = self->procAt(i)->Wrap();
  }

  // Passing result by calling function with arguments instead of populating
  // and returning v8::Array since it's so much faster
  // This technique came from node: https://github.com/joyent/node/blob/5344d0c1034b28f9e6de914430d8c8436ad85105/src/node_file.cc#L326
  // (thanks @trevnorris for explaining it to me)
  cb->NewInstance(self->_len, argv);

  return v8::Undefined();
}

v8::Handle<v8::Value> Procjs::Refresh(const v8::Arguments& args) {
  using namespace v8;
  Isolate *isolate = args.GetIsolate();
  HandleScope handle_scope;

  // our JS API ensures we always pass flags (defaults if none were given)
  Local<Integer> flags = args[0].As<Integer>();
  assert(flags->IsUint32());

  Procjs* self = Unwrap<Procjs>(args);
  self->refresh(flags->Uint32Value());

  return v8::Undefined();
}
