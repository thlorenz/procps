'use strict';

var test = require('tap').test
var getPidDigits = require('../').sysinfo.getPidDigits

test('\nget pid digits', function (t) {
  var r = getPidDigits();
  t.equal(r, 5, 'returns 5')
  t.end()
})
