#include <nan.h>
#include "procps.h"

using v8::ObjectTemplate;
using v8::Uint32;

#define X(prop, Prop)                                              \
  NAN_GETTER(SlabCache::Prop) {                                    \
    NanScope();                                                    \
                                                                   \
    SlabCache* self = Unwrap<SlabCache>(args);                     \
    NanReturnValue(NanNew<Uint32>(self->_slab.prop));              \
  }


// unsigned int
X(active_objs, activeObjs)
X(num_objs, numObjs)
X(objsize, objsize)
X(objperslab, objperslab)

#undef X

// char[48]
NAN_GETTER(SlabCache::name) {
  NanScope();
  SlabCache* self = Unwrap<SlabCache>(args);
  NanReturnValue(NanNew<String>(self->_slab.name));
}

Local<Value> SlabCache::Wrap() {

  NanEscapableScope();

  Handle<ObjectTemplate> t = ObjectTemplate::New();
  t->SetInternalFieldCount(1);

#define X(prop) \
  t->SetAccessor(NanNew<String>(#prop), SlabCache::prop);

  X(activeObjs)
  X(numObjs)
  X(objsize)
  X(objperslab)
  X(name)

#undef X

  Local<Object> instance = t->NewInstance();
  instance->SetInternalField(0, NanNew<External>(this));

  return NanEscapeScope(instance);
}
