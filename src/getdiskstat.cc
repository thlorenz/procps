#include <nan.h>
#include "procps.h"
#include "proc/sysinfo.h"

using v8::ObjectTemplate;
using v8::Uint32;

NAN_GETTER(GetDiskStat::Disks) {
  NanScope();
  GetDiskStat* self = Unwrap<GetDiskStat>(args);

  Local<Array> wrapDisks = NanNew<Array>(self->_ndisks);

  for (int i = 0; i < self->_ndisks; i++) {
    DiskStat *stat = new DiskStat(self->_disks[i]);
    wrapDisks->Set(i, stat->Wrap());
  }

  NanReturnValue(wrapDisks);
}

NAN_GETTER(GetDiskStat::Partitions) {
  NanScope();

  GetDiskStat* self = Unwrap<GetDiskStat>(args);
  partition_stat *partitions = self->_partitions;

  int npartitions = sizeof(partitions) / sizeof(partition_stat);
  Local<Array> wrapPartitions = NanNew<Array>(npartitions);

  for (int i = 0; i < npartitions; i++) {
    PartitionStat *stat = new PartitionStat(partitions[i]);
    wrapPartitions->Set(i, stat->Wrap());
  }

  NanReturnValue(wrapPartitions);
}

Local<Value> GetDiskStat::Wrap() {
  NanEscapableScope();

  Handle<ObjectTemplate> t = ObjectTemplate::New();
  t->SetInternalFieldCount(1);

  t->SetAccessor(NanNew<String>("disks"), GetDiskStat::Disks);
  t->SetAccessor(NanNew<String>("partitions"),GetDiskStat::Partitions);

  Local<Object> instance = t->NewInstance();
  instance->SetInternalField(0, NanNew<External>(this));

  return NanEscapeScope(instance);
}
