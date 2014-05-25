'use strict';

var procps = require('../');

var proctab = procps.readproctab();
var bycommand = proctab.reduce(function (acc, p) {
  acc[p.cmd] = p;
  return acc;
}, {});

console.log(bycommand.node)
