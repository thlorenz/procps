'use strict';

function inspect(obj, depth) {
  console.error(require('util').inspect(obj, false, depth || 5, true));
}

var procjs = require('../');

var proctab = procjs.readproctab();
var bycmd = proctab.reduce(function (acc, p) {
  acc[p.cmd] = p;
  return acc;
}, {})
inspect(bycmd)
