'use strict';

var sysinfo = require('../').sysinfo;
var time = sysinfo.uptime();

console.log('uptime: %d secs, idle: %d secs', time.uptime, time.idle);

var timeSince = sysinfo.uptimeSince();
console.log('up since:\n', timeSince);
