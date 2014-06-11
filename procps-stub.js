'use strict';

exports.readproctab = function (_, cb) {
  cb();
}

exports.sysinfo_meminfo = function (_, cb) {
  cb();  
}

exports.sysinfo_vminfo = function (cb) {
  cb();
}

exports.sysinfo_Hertz = function () {
  return 0;
}

exports.sysinfo_getstat = function (cb) {
  cb();
}

exports.sysinfo_getdiskstat = function (cb) {
  cb([], []);
}

exports.sysinfo_uptime = function (cb) {
  cb();
}

exports.sysinfo_uptimesince = function (cb) {
  cb();
}

exports.sysinfo_uptimestring = function (cb) {
  cb('');
}

exports.sysinfo_loadavg = function (cb) {
  cb();
}

exports.sysinfo_getpiddigits = function (cb) {
  cb();
}

exports.sysinfo_getslabinfo = function (cb) {
  cb();
}
