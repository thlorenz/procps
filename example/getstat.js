'use strict';

function inspect(obj, depth) {
  console.error(require('util').inspect(obj, false, depth || 5, true));
}

var sysinfo = require('../').sysinfo;

console.log('Hertz: %d', sysinfo.Hertz);

var res = sysinfo.getstat();
inspect(res);
console.log('%d forks', res.processes);
