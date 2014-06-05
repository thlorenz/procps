#include <nan.h>
#include "procps.h"

using v8::ObjectTemplate;
using v8::Uint32;

#define X(prop, Prop)                                            \
  NAN_GETTER(DiskStat::Prop) {                                   \
    NanScope();                                                  \
                                                                 \
    DiskStat* self = Unwrap<DiskStat>(args);                     \
    NanReturnValue(NanNew<Uint32>((uint32_t) self->_stat.prop)); \
  }

// unsigned long long
X(reads_sectors, readsSectors)
X(written_sectors, writtenSectors)

// unsigned
X(inprogress_IO, inprogressIO)
X(merged_reads, mergedReads)
X(merged_writes, mergedWrites)
X(milli_reading, milliReading)
X(milli_spent_IO, milliSpentIO)
X(milli_writing, milliWriting)
X(partitions, partitions)
X(reads, reads)
X(weighted_milli_spent_IO, weightedMilliSpentIO)
X(writes, writes)

#undef X

// char disk_name [16];
NAN_GETTER(DiskStat::diskName) {
  NanScope();
  DiskStat* self = Unwrap<DiskStat>(args);
  NanReturnValue(NanNew<String>(self->_stat.disk_name));
}

Local<Value> DiskStat::Wrap() {

  NanEscapableScope();

  Handle<ObjectTemplate> t = ObjectTemplate::New();
  t->SetInternalFieldCount(1);

#define X(prop) \
  t->SetAccessor(NanNew<String>(#prop), DiskStat::prop);

  X(readsSectors)
  X(writtenSectors)

  X(inprogressIO)
  X(mergedReads)
  X(mergedWrites)
  X(milliReading)
  X(milliSpentIO)
  X(milliWriting)
  X(partitions)
  X(reads)
  X(weightedMilliSpentIO)
  X(writes)
  X(diskName)

#undef X

  Local<Object> instance = t->NewInstance();
  instance->SetInternalField(0, NanNew<External>(this));

  return NanEscapeScope(instance);
}
