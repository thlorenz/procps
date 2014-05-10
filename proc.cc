#include "procjs.h"

#define X(Prop, prop, convert)                                             \
  void Proc::Prop(                                                         \
      v8::Local<v8::String> property,                                      \
      const v8::PropertyCallbackInfo<v8::Value>& info) {                   \
    using namespace v8;                                                    \
    Isolate *isolate = info.GetIsolate();                                  \
    HandleScope handle_scope(isolate);                                     \
                                                                           \
    Proc* self = Unwrap<Proc>(info);                                       \
    info.GetReturnValue().Set(convert(isolate, self->_proc->prop));        \
  }

X(Ppid, ppid, Number::New)
X(Tid, tid, Number::New)
X(Cmd, cmd, String::NewFromUtf8)

#undef X

void Proc::Cmdline(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
  using namespace v8;
  Isolate *isolate = info.GetIsolate();
  HandleScope handle_scope(isolate);

  Proc* self= Unwrap<Proc>(info);
  proc_t *_proc = self->_proc;

  Local<Array> arr;

  if (_proc->cmdline) {
    int len = 0;
    while (_proc->cmdline[len]) len++;

    arr = Array::New(self->_isolate, len);

    int i = 0;
    for (i = 0; i < len; i++) {
      arr->Set(Integer::New(self->_isolate, i), String::NewFromUtf8(self->_isolate, _proc->cmdline[i]));
    }
  } else {
    arr = Array::New(self->_isolate, 0);
  }

  info.GetReturnValue().Set(arr);
}


v8::Handle<v8::Object> Proc::Wrap() {
  using namespace v8;

  // HandleScope handle_scope(_isolate);
  Handle<ObjectTemplate> t = ObjectTemplate::New(_isolate);
  t->SetInternalFieldCount(1);

#define X(Prop, prop) \
  t->SetAccessor(String::NewFromUtf8(_isolate, #prop), Proc::Prop);

  X(Ppid, ppid)
  X(Tid, tid)
  X(Cmd, cmd)
  X(Cmdline, cmdline)

#undef X

  Local<Object> instance = t->NewInstance();
  instance->SetInternalField(0, External::New(_isolate, this));

  return instance;
}
