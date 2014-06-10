'use strict';

var test = require('tap').test
var getdiskstat = require('../').sysinfo.getdiskstat

function inspect(obj, depth) {
  console.error(require('util').inspect(obj, false, depth || 5, true));
}

// on travis there is no access to /proc/diskstats (http://askubuntu.com/a/257517/53802)
if (!process.env.TRAVIS)
test('\ngetdiskstat', function (t) {
  var r = getdiskstat();
  var disks = r.disks
    , parts = r.partitions;

  t.ok(disks.length >= 2, 'returns at least 2 disks')
  t.ok(parts.length >= 2, 'returns at least 2 partitionss')

  var d = disks[0];

  t.equal(d.diskName, 'sda', 'sda is first disk')
  t.ok(d.writes > 0, 'sda has writes')
  t.ok(d.reads > 0, 'sda has reads')
  t.ok(d.weightedMilliSpentIO > 0, 'sda has weightedMilliSpentIO')
  t.ok(d.partitions > 0, 'sda has partitions')
  t.ok(d.milliWriting > 0, 'sda has milliWriting')
  t.ok(d.readsSectors > 0, 'sda has readsSectors')
  t.ok(d.writtenSectors > 0, 'sda has writtenSectors')

  var p = parts[0];
  var pp = parts[1];

  t.equal(p.partitionName, 'sda1', 'sda1 is first partition')
  t.equal(pp.partitionName, 'sda2', 'sda2 is secod partition')

  t.equal(p.parentDisk, 0, 'parent disk of first partition is sda')
  t.equal(pp.parentDisk, 0, 'parent disk of second partition is sda')
  t.ok(p.writes + pp.writes <= d.writes, 'partitions writes are no more than disk writes')
  t.ok(p.reads + pp.reads <= d.reads, 'partitions reads are no more than disk reads')
  
  t.end()
})
