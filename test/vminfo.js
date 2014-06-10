'use strict';

var test = require('tap').test
var vminfo = require('../').sysinfo.vminfo

function inspect(obj, depth) {
  console.error(require('util').inspect(obj, false, depth || 5, true));
}

test('\nvminfo', function (t) {
  var r = vminfo();  
  t.ok(isFinite(r.nrDirty), 'has number nrDirty')
  t.ok(isFinite(r.nrMapped), 'has number zero nrMapped')
  t.ok(isFinite(r.pgpgin), 'has pgpgin')
  t.ok(r.pgpgout > 0, 'has non zero pgpgout')
  t.ok(isFinite(r.pgalloc), 'has number pgalloc')
  t.ok(isFinite(r.pgfree), 'has number pgfree')
  t.ok(isFinite(r.pgactivate), 'has number pgactivate')
  t.ok(isFinite(r.pgfault), 'has number pgfault')
  t.ok(isFinite(r.pgmajfault), 'has number pgmajfault')
  t.end()
})
