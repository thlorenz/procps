#include <nan.h>
#include "procps.h"

using v8::ObjectTemplate;
using v8::Uint32;

#define X(prop, Prop)                                            \
  NAN_GETTER(PartitionStat::Prop) {                              \
    NanScope();                                                  \
                                                                 \
    PartitionStat* self = Unwrap<PartitionStat>(args);           \
    NanReturnValue(NanNew<Uint32>((uint32_t) self->_stat.prop)); \
  }

// unsigned long long
X(reads_sectors, readsSectors)
// unsigned
X(parent_disk, parentDisk)
X(reads, reads)
X(writes, writes)
X(requested_writes, requestedWrites)

#undef X

// char partition_name [16];
NAN_GETTER(PartitionStat::partitionName) {
  NanScope();
  PartitionStat* self = Unwrap<PartitionStat>(args);
  NanReturnValue(NanNew<String>(self->_stat.partition_name));
}

Local<Value> PartitionStat::Wrap() {

  NanEscapableScope();

  Handle<ObjectTemplate> t = ObjectTemplate::New();
  t->SetInternalFieldCount(1);

#define X(prop) \
  t->SetAccessor(NanNew<String>(#prop), PartitionStat::prop);

  X(readsSectors)

  X(parentDisk)
  X(reads)
  X(writes)
  X(requestedWrites)
  X(partitionName)

#undef X

  Local<Object> instance = t->NewInstance();
  instance->SetInternalField(0, NanNew<External>(this));

  return NanEscapeScope(instance);
}
