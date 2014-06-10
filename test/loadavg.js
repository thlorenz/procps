'use strict';

var test = require('tap').test
var loadavg = require('../').sysinfo.loadavg
var fs = require('fs')

function inspect(obj, depth) {
  console.error(require('util').inspect(obj, false, depth || 5, true));
}

test('\nloadavg', function (t) {
  var r = loadavg();
  var avgs = fs.readFileSync('/proc/loadavg', 'utf8').split(' ');

  t.equal(r.length, 3, 'returns averages for 1, 5 and 15 minutes')
  t.equal(r[0], parseFloat(avgs[0]), '1 minute average is correct')
  t.equal(r[1], parseFloat(avgs[1]), '5 minute average is correct')
  t.equal(r[2], parseFloat(avgs[2]), '15 minute average is correct')
  t.end()
  
})
