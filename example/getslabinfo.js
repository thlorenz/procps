'use strict';

var sysinfo = require('../').sysinfo;

console.log('get slab info requires you to be root, there for use sudo to run this example');
console.log(sysinfo.getslabinfo());
