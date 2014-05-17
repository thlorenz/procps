'use strict';

function inspect(obj, depth) {
  console.error(require('util').inspect(obj, false, depth || 5, true));
}

var Procjs = require('../');
var procjs = Procjs.create();

/*console.log(Object.keys(procjs.procs[0]).join(', '));
console.log(procjs.procs[0].cmd);

var p;
for (var i = 0; i < procjs.procs.length; i++) {
  p = procjs.procs[i];
  process.stdout.write('[ ' + p.ppid + ':' + p.cmd + ' ]');
}*/
