'use strict';

var logs = [];

function logProcs(pjs) {
  var s;
  for (var i  = 0; i < pjs.procs.length; i++) {
    var ps = pjs.procs[i];
    s = ps.ppid + ' tid: ' + ps.tid + ': ' + ps.cmd;
    if (ps.cmdline && ps.cmdline.length) {
      s += ' [ ';
      for (var j = 0; j < ps.cmdline.length; j++) s += ps.cmdline[j] + ' ';
      s += ' ]';
    }
    logs.push(s);
  }
}

var pjs = new Procjs();
logProcs(pjs);

pjs.refresh();
logs.push('\n==============================\n');
logProcs(pjs);

(function() { return logs.join('\n'); })();
