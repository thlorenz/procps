'use strict';

var test = require('tap').test
var hertz = require('../').sysinfo.Hertz

test('\nHertz', function (t) {
  t.equal(hertz, 100, 'returns 100 hertz')
  t.end()
})
