#include "procjs.h"
#include <cassert>

#define MAX_PROCS 5000

static void AddFunction(v8::Isolate* isolate, v8::Handle<v8::Object> global, const char* name, v8::FunctionCallback callback) {
  using namespace v8;
  HandleScope handle_scope(isolate);
  global->Set(String::NewFromUtf8(isolate, name), FunctionTemplate::New(isolate, callback)->GetFunction());
}

void Procjs::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
  using namespace v8;
  Isolate *isolate = args.GetIsolate();
  HandleScope handle_scope(isolate);

  // our JS API ensures we always pass flags (defaults if none were given)
  Local<Integer> flags = args[0].As<Integer>();
  assert(flags->IsUint32());

  Handle<ObjectTemplate> t = ObjectTemplate::New();
  t->SetInternalFieldCount(1);

  Procjs *self = new Procjs(isolate, flags->Uint32Value());
  Local<Object> instance = t->NewInstance();
  instance->SetInternalField(0, External::New(isolate, self));

  AddFunction(isolate, instance, "procs", Procjs::Procs);
  AddFunction(isolate, instance, "refresh", Procjs::Refresh);

  args.GetReturnValue().Set(instance);
}

void Procjs::Procs(const v8::FunctionCallbackInfo<v8::Value>& args) {
  using namespace v8;
  Isolate *isolate = args.GetIsolate();
  HandleScope handle_scope(isolate);

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
}

void Procjs::Refresh(const v8::FunctionCallbackInfo<v8::Value>& args) {
  using namespace v8;
  Isolate *isolate = args.GetIsolate();
  HandleScope handle_scope(isolate);

  // our JS API ensures we always pass flags (defaults if none were given)
  Local<Integer> flags = args[0].As<Integer>();
  assert(flags->IsUint32());

  Procjs* self = Unwrap<Procjs>(args);
  self->refresh(flags->Uint32Value());
}
