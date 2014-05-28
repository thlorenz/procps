'use strict';

var test = require('tap').test
var os = require('os')
var meminfo = require('../').sysinfo.meminfo

function inspect(obj, depth) {
  console.error(require('util').inspect(obj, false, depth || 5, true));
}

test('\ncalling meminfo without a unit', function (t) {
  var info = meminfo();  
  var osi = { totalmem: os.totalmem() }

  t.equal(info.mainTotal, osi.totalmem, 'main total is total memory')
  t.ok(0 < info.mainBuffers < info.mainTotal, 'non-zero main buffers is less than total')
  t.ok(0 < info.mainCached < info.mainTotal, 'non-zero main cached is less than total')
  t.ok(0 < info.mainFree < info.mainTotal, 'non-zero main free is less than total')
  t.ok(0 < info.mainUsed < info.mainTotal, 'non-zero main used is less than total')

  t.equal(info.lowFree, info.mainFree, 'low free is same as main free')
  t.equal(info.lowTotal, info.mainTotal, 'low total is same as main total')

  t.ok(0 < info.swapFree < info.swapTotal, 'non zero swap free is less than swap total')
  t.ok(0 < info.active, 'non zero active')

  t.equal(info.highFree, 0, 'highFree is zero')
  t.equal(info.highTotal, 0, 'highTotal is zero')

  t.equal(info.inactLaundry, 0, 'inactLaundry is zero')
  t.equal(info.inactDirty, 0, 'inactDirty is zero')
  t.equal(info.inactClean, 0, 'inactClean is zero')
  t.equal(info.inactTarget, 0, 'inactTarget is zero')
  ;

  // not too sure about these, they are all 0 on my machine
  [ 'swapCached', 'swapUsed', 'writeback', 'reversemaps', 'dirty',
    'committedAs', 'mapped', 'pagetables'
  ].forEach(function (k) { 
      t.ok(isFinite(info[k]), 'includes ' + k)
    });

  [ 'slab', 'inactive'
  ].forEach(function (k) { 
      t.ok(0 < info[k], 'includes non zero ' + k)
    });

  t.end()
})

test('\ncalling meminfo with unit k', function (t) {
  var info = meminfo();
  var infok = meminfo('k');  
  var expectedTotal = Math.floor(info.mainTotal / Math.pow(2, 10));
  var diffTotal = Math.abs(expectedTotal - infok.mainTotal) / infok.mainTotal;
  t.ok(diffTotal < 0.00001 , 'main total is returned in kb')
  t.end()
})

test('\ncalling meminfo with unit m', function (t) {
  var info = meminfo();
  var infom = meminfo('m');  
  var expectedTotal = Math.floor(info.mainTotal / Math.pow(2, 20));
  var diffTotal = Math.abs(expectedTotal - infom.mainTotal) / infom.mainTotal;
  t.ok(diffTotal < 0.00001 , 'main total is returned in mb')
  t.end()
})
