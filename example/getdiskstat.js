'use strict';

function inspect(obj, depth) {
  console.error(require('util').inspect(obj, false, depth || 5, true));
}

var sysinfo = require('../').sysinfo;
var stat = sysinfo.getdiskstat();

inspect(stat);

console.log('%d disks', stat.disks.length)
console.log('%d partitions', stat.partitions.length)
