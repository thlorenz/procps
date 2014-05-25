'use strict';

var procps = require('./build/Release/procps');

var readprocFlags = {
    PROC_FILLMEM     : 0x0001 // read statm
  , PROC_FILLCOM     : 0x0002 // alloc and fill in `cmdline'
  , PROC_FILLENV     : 0x0004 // alloc and fill in `environ'
  , PROC_FILLUSR     : 0x0008 // resolve user id number -> user name
  , PROC_FILLGRP     : 0x0010 // resolve group id number -> group name
  , PROC_FILLSTATUS  : 0x0020 // read status -- currently unconditional
  , PROC_FILLSTAT    : 0x0040 // read stat -- currently unconditional
  , PROC_FILLWCHAN   : 0x0080 // look up WCHAN name
  , PROC_FILLARG     : 0x0100 // alloc and fill in `cmdline'
  , PROC_LOOSE_TASKS : 0x0200 // threat threads as if they were processes
};

var readprocFlagsFillAll = 0
  | readprocFlags.PROC_FILLMEM
  | readprocFlags.PROC_FILLCOM
  | readprocFlags.PROC_FILLENV
  | readprocFlags.PROC_FILLUSR
  | readprocFlags.PROC_FILLGRP
  | readprocFlags.PROC_FILLSTATUS
  | readprocFlags.PROC_FILLSTAT
  | readprocFlags.PROC_FILLWCHAN
  | readprocFlags.PROC_FILLARG
  | readprocFlags.PROC_LOOSE_TASKS
  ;

exports.readproctab = function (flags_) {
  var args;
  var flags = typeof flags_ === 'undefined' ? readprocFlagsFillAll : flags_;

  // passing result as args array instead of v8::Array return value
  // the callback is invoked synchronously
  procps.readproctab(flags, function () { args = Array.prototype.slice.call(arguments); });
  return args;
}
exports.readprocFlags = readprocFlags;
exports.readprocFlagsFillAll = readprocFlagsFillAll;
