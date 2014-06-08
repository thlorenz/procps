'use strict';

var sysinfo = require('../').sysinfo;
var time = sysinfo.uptime();

console.log('uptime: %d secs, idle: %d secs', time.uptime, time.idle);

var timeSince = sysinfo.uptimeSince();
console.log('up since:\n', timeSince);

console.log('human readable uptime string "%s"', sysinfo.uptimeString(true));
console.log('all inclusive uptime string "%s"', sysinfo.uptimeString());
