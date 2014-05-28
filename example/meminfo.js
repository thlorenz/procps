'use strict';

function inspect(obj, depth) {
  console.error(require('util').inspect(obj, false, depth || 5, true));
}

var meminfo = require('../').sysinfo.meminfo;
inspect(meminfo());

var inmb = meminfo('m');
Object.keys(inmb).forEach(function (k) { inmb[k] += ' MB' });
inspect(inmb);
