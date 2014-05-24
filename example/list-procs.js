'use strict';

function inspect(obj, depth) {
  console.error(require('util').inspect(obj, false, depth || 5, true));
}

var Procjs = require('../');
inspect(Procjs.procFlags);

var procjs = Procjs.create();

var p;
for (var i = 0; i < procjs.procs.length; i++) {
  p = procjs.procs[i];
  Object.keys(p).forEach(function (k) {
    if (k === 'environ') return;
    process.stdout.write(k + ': ' + p[k] + ' | ');
  })
  process.stdout.write('\n\n');
}
