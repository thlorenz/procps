#include "procjs.h"

static void AddFunction(v8::Isolate* isolate, v8::Handle<v8::Object> global, const char* name, v8::FunctionCallback callback) {
  using namespace v8;
  HandleScope handle_scope(isolate);
  global->Set(String::NewFromUtf8(isolate, name), FunctionTemplate::New(isolate, callback)->GetFunction());
}

void Procjs::New(const v8::FunctionCallbackInfo<v8::Value>& info) {
  using namespace v8;
  Isolate *isolate = info.GetIsolate();
  HandleScope handle_scope(isolate);

  Handle<ObjectTemplate> t = ObjectTemplate::New();
  t->SetInternalFieldCount(1);
  t->SetAccessor(String::NewFromUtf8(isolate, "procs"), Procjs::Procs);

  Procjs *self = new Procjs(isolate);
  Local<Object> instance = t->NewInstance();
  instance->SetInternalField(0, External::New(isolate, self));

  AddFunction(isolate, instance, "refresh", Procjs::Refresh);

  info.GetReturnValue().Set(instance);
}

void Procjs::Procs(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
  using namespace v8;
  Isolate *isolate = info.GetIsolate();
  HandleScope handle_scope(isolate);

  Procjs* self = Unwrap<Procjs>(info);
  Local<Array> procs = Array::New(self->_isolate, self->_len);

  for (int i = 0; i < self->_len; i++) {
    procs->Set(Integer::New(self->_isolate, i), self->procAt(i)->Wrap());
  }

  info.GetReturnValue().Set(procs);
}

void Procjs::Refresh(const v8::FunctionCallbackInfo<v8::Value>& info) {
  using namespace v8;
  Isolate *isolate = info.GetIsolate();
  HandleScope handle_scope(isolate);

  Procjs* self = Unwrap<Procjs>(info);
  self->refresh();
}
