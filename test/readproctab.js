'use strict';

var test = require('tap').test
var readproctab = require('../').readproctab

function bycmd(proctab) {
  return proctab.reduce(function (acc, p) {
    acc[p.cmd] = p;
    return acc;
  }, {})
}

function inspect(obj, depth) {
  console.error(require('util').inspect(obj, false, depth || 5, true));
}

test('\nwhen reading proctab with default flags', function (t) {
  var pt = readproctab();
  var bc = bycmd(pt);
  var proc = bc.node;
  var env = process.env;

  t.ok(pt.length > 10, 'returns at least 10 processes')

  t.ok(~proc.environ.indexOf('HOME=' + env.HOME), 'includes HOME in environ for this process')
  t.ok(~proc.environ.indexOf('MAIL=' + env.MAIL), 'includes MAIL in environ for this process')
  t.ok(~proc.environ.indexOf('SHELL=' + env.SHELL), 'includes SHELL in environ for this process')
  t.ok(~proc.environ.indexOf('HOME=' + env.HOME), 'includes HOME in environ for this process')
  t.ok(~proc.environ.indexOf('MAIL=' + env.MAIL), 'includes MAIL in environ for this process')

  t.similar(proc.cmdline[0], /node/, 'includes first cmdline part')
  t.similar(proc.cmdline, /test\/readproctab/, 'includes second cmdline part')

  t.similar(proc.cmd, /node/, 'includes correct cmd')

  t.equal(proc.tgid, process.pid, 'tgid  matches process.pid')
  t.equal(proc.tid, process.pid, 'tid  matches process.pid')

  t.equal(proc.fuser, env.USER, 'sets correct fuser')
  t.equal(proc.suser, env.USER, 'sets correct suser')
  t.equal(proc.ruser, env.USER, 'sets correct ruser')
  t.equal(proc.euser, env.USER, 'sets correct euser')

  t.ok(proc.fuid > 0, 'has valid fuid')
  t.ok(proc.suid > 0, 'has valid suid')
  t.ok(proc.ruid > 0, 'has valid ruid')
  t.ok(proc.euid > 0, 'has valid euid')

  t.equal(proc.fgid, proc.fuid, 'fgid === fuid')
  t.equal(proc.sgid, proc.suid, 'sgid === suid')
  t.equal(proc.rgid, proc.ruid, 'rgid === ruid')
  t.equal(proc.egid, proc.euid, 'egid === euid')

  t.end();
})

test('\nwhen reading with flags that exclude FILLENV and FILLUSR', function (t) {
  var flags = readproctab.flagsFillAll 
    ^ readproctab.flags.PROC_FILLENV 
    ^ readproctab.flags.PROC_FILLUSR;

  var pt = readproctab(flags);
  var bc = bycmd(pt);
  var proc = bc.node;

  t.ok(pt.length > 10, 'returns at least 10 processes')
  t.ok(!proc.env, 'does not include environ')

  t.similar(proc.cmdline[0], /node/, 'includes first cmdline part')
  t.similar(proc.cmdline, /test\/readproctab/, 'includes second cmdline part')

  t.similar(proc.cmd, /node/, 'includes correct cmd')

  t.equal(proc.tgid, process.pid, 'tgid  matches process.pid')
  t.equal(proc.tid, process.pid, 'tid  matches process.pid')

  t.ok(!proc.fuser, 'sets no fuser')
  t.ok(!proc.suser, 'sets no suser')
  t.ok(!proc.ruser, 'sets no ruser')
  t.ok(!proc.euser, 'sets no euser')

  t.ok(proc.fuid > 0, 'has valid fuid')
  t.ok(proc.suid > 0, 'has valid suid')
  t.ok(proc.ruid > 0, 'has valid ruid')
  t.ok(proc.euid > 0, 'has valid euid')

  t.equal(proc.fgid, proc.fuid, 'fgid === fuid')
  t.equal(proc.sgid, proc.suid, 'sgid === suid')
  t.equal(proc.rgid, proc.ruid, 'rgid === ruid')
  t.equal(proc.egid, proc.euid, 'egid === euid')

  t.end()
})
