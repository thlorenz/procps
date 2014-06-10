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
  t.ok(isFinite(r.cpuIow), 'has cpuIow')
  t.ok(isFinite(r.cpuXxx), 'has cpuXxx')
  t.ok(isFinite(r.cpuYyy), 'has cpuYyy')
  t.ok(isFinite(r.cpuZzz), 'has cpuZzz')
  t.ok(isFinite(r.pgpgin), 'has pgpgin')
  t.ok(r.pgpgout > 0, 'non zero pgpgout')
  t.ok(isFinite(r.intr), 'has intr')
  t.ok(r.ctxt > 0, 'non zero ctxt')
  t.ok(r.btime > 0, 'non zero btime')
  t.ok(r.processes > 0, 'non zero processes')

  t.end()
})
