'use strict';

var test = require('tap').test
var getstat = require('../').sysinfo.getstat

function inspect(obj, depth) {
  console.error(require('util').inspect(obj, false, depth || 5, true));
}

test('\ngetstat', function (t) {
  var r = getstat();

  t.ok(r.cpuUse > 0, 'non zero cpuUse')
  t.ok(r.cpuSys > 0, 'non zero cpuSys')
  t.ok(r.cpuIdl > 0, 'non zero cpuIdl')
  t.ok(r.cpuIow > 0, 'non zero cpuIow')
  t.ok(r.cpuXxx > 0, 'non zero cpuXxx')
  t.ok(r.cpuYyy > 0, 'non zero cpuYyy')
  t.ok(r.pgpgin > 0, 'non zero pgpgin')
  t.ok(r.pgpgout > 0, 'non zero pgpgin')
  t.ok(r.intr > 0, 'non zero intr')
  t.ok(r.ctxt > 0, 'non zero ctxt')
  t.ok(r.btime > 0, 'non zero btime')
  t.ok(r.processes > 0, 'non zero processes')

  t.end()
})
