'use strict';

var logs = [];

var pjs = new Procjs();
var s;

for (var i  = 0; i < pjs.procs.length; i++) {
  var ps = pjs.procs[i];
  s = ps.pid + ': ' + ps.cmd;
  if (ps.cmdline && ps.cmdline.length) {
    s += '[ ';
    for (var j = 0; j < ps.cmdline.length; j++) s += ps.cmdline[j] + ' ';
    s += ' ]';
  }
  logs.push(s);
}

(function() { return logs.join('\n'); })();
