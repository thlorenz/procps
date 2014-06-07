'use strict';

var procps = require('./build/Release/procps');

/*
 * readproc
 */

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

/**
 * Calls underlying readproctab and returns results with the following ajustments:
 *
 * - underscore_names are camel cased
 * - Int64 values are converted to Int32 values
 *
 * @name readproctab
 * @function
 * @param {number=} flags_ flags passed to readproc, allow filling specific process properties only instead of all of them which is the default
 * @return {Array.<Object>} information about all processes running on the system
 */
var readproctab = exports.readproctab = function (flags_) {
  var args;
  var flags = typeof flags_ === 'undefined' ? readprocFlagsFillAll : flags_;

  // passing result as args array instead of v8::Array return value
  // the callback is invoked synchronously
  procps.readproctab(flags, function () { args = Array.prototype.slice.call(arguments); });
  return args;
}

/**
 * A hashtable of all readproc flags.
 * Use these in order to fill/loose specific process properties.
 *
 * @name readproctab::flags
 */
readproctab.flags = readprocFlags;

/**
 * The flags used by default which cause readproc to fill all properties of each process.
 * Use them as a starting point to turn properties off selectively, i.e.:
 *
 * ```js
 * var flags = readproctab.flagsFillAll ^ readproctab.flags.PROC_FILLENV ^ readproctab.flags.PROC_FILLUSR;
 * ```
 *
 * @name readproctab::flagsFillAll
 */
readproctab.flagsFillAll = readprocFlagsFillAll;

/*
 * sysinfo
 */

var shifts = {
  'b': 0, 'k': 10, 'm': 20, 'g': 30
};

var sysinfo = exports.sysinfo = {};

/**
 * A hybrid of `procps.meminfo` and `free`.
 *
 * @name sysinfo::meminfo
 * @function
 * @param {string=} unit `'b'|'k'|'m'|'g'` to return usage in Bytes|KB|MB|GB respectively
 * @return {Object} with properties indicating memory usage, like `mainTotal`
 */
sysinfo.meminfo = function meminfo (unit) {

  var shift = unit ? shifts[unit.toLowerCase()] || 0 : 0;
  var args;
  procps.sysinfo_meminfo(shift, function () { args = arguments; });

  return [
    'mainBuffers'
  , 'mainCached'
  , 'mainFree'
  , 'mainTotal'
  , 'swapFree'
  , 'swapTotal'
  , 'highFree'
  , 'highTotal'
  , 'lowFree'
  , 'lowTotal'
  , 'active'
  , 'inactLaundry'
  , 'inactDirty'
  , 'inactClean'
  , 'inactTarget'
  , 'swapCached'
  , 'swapUsed'
  , 'mainUsed'
  , 'writeback'
  , 'slab'
  , 'reversemaps'
  , 'committedAs'
  , 'dirty'
  , 'inactive'
  , 'mapped'
  , 'pagetables'
  ].reduce(function (acc, k, idx) {
      acc[k] = args[idx];
      return acc;
    }, {})
}

function inspect(obj, depth) {
  console.error(require('util').inspect(obj, false, depth || 5, true));
}

/*
 * Some values in /proc are expressed in units of 1/HZ seconds, where HZ
 * is the kernel clock tick rate.
 *
 * @name sysinfo::Hertz
 */
sysinfo.Hertz = procps.sysinfo_Hertz();

/**
 * Gets statistics about cpu, process and memory usage.
 * `procps.getstat` used by various `vmstat` functions.
 *
 * Includes btime therefore sysinfo.getbtime is not implemented separately.
 *
 * @name sysinfo::getstat
 * @function
 * @return {Object} with the following properties:
 *
 * - **cpuUse**:    non-nice user cpu ticks
 * - **cpuNic**:    nice user cpu ticks
 * - **cpuSys**:    system cpu ticks
 * - **cpuIdl**:    idle cpu ticks
 * - **cpuIow**:    IO-wait cpu ticks
 * - **cpuXxx**:    IRQ cpu ticks
 * - **cpuYyy**:    softirq cpu ticks
 * - **cpuZzz**:    stolen irq ticks
 * - **pgpgin**:    pages paged in
 * - **pgpgout**:   pages paged out
 * - **pswpin**:    pages swapped in
 * - **pswpout**:   pages swapped out
 * - **intr**:      interrupts
 * - **ctxt**:      CPU context switches
 * - **running**:   processes running
 * - **blocked**:   processes blocked
 * - **btime**:     boot time
 * - **processes**: forks
 *
 */
sysinfo.getstat = function getstat() {
  var args;
  procps.sysinfo_getstat(function () { args = arguments; });

  var acc = {}, idx;

  [ 'cpuUse'
  , 'cpuNic'
  , 'cpuSys'
  , 'cpuIdl'
  , 'cpuIow'
  , 'cpuXxx'
  , 'cpuYyy'
  , 'cpuZzz'
  , 'pgpgin'
  , 'pgpgout'
  , 'pswpin'
  , 'pswpout'
  , 'intr'
  , 'ctxt'
  , 'running'
  , 'blocked'
  , 'btime'
  , 'processes'
  ].reduce(function (acc, k, idx) {
    acc[k] = args[idx];
    return acc;
  }, acc)

  return acc;
}

/**
 * Gets statistics about disks/devices and partitions on the machine.
 *
 * ##### Example DiskStats
 *
 *```js
 * { disks:
 *  [ { diskName: 'sda',
 *      writes: 51770,
 *      weightedMilliSpentIO: 121633,
 *      reads: 14706,
 *      partitions: 2,
 *      milliWriting: 102280,
 *      milliSpentIO: 24633,
 *      milliReading: 19366,
 *      mergedWrites: 131130,
 *      mergedReads: 3164,
 *      inprogressIO: 0,
 *      writtenSectors: 1554100,
 *      readsSectors: 486100 },
 *    { diskName: 'loop0',
 *      ...
 *   partitions:
 *    [ { partitionName: 'sda1',
 *        requestedWrites: 1554100,
 *        writes: 51693,
 *        reads: 14553,
 *        parentDisk: 0,
 *        readsSectors: 483762 },
 *      { partitionName: 'sda2',
 *      ...
 *    ]}
 *```
 *
 * @name sysinfo::getdiskstat
 * @function
 * @return {Object} with `disks` array and `partitions` array
 *
 */
sysinfo.getdiskstat = function getdiskstat() {
  var args;
  procps.sysinfo_getdiskstat(function () { args = arguments; });

  return { disks: args[0], partitions: args[1] };
}

sysinfo.uptime = function uptime() {
  var args;
  procps.sysinfo_uptime(function () { args = arguments; });

  return { uptime: args[0], idle: args[1] };
}

/**
 * Returns uptime since structured into years, months, etc. for easy logging.
 * Very similar to `uptime -s` command.
 *
 * @name uptimeSince
 * @function
 * @return {Object} with the following properties:
 *
 * - **year**: Year	- 1900
 * - **mon **: Month	[0-11]
 * - **mday**: Day		[1-31]
 * - **hour**: Hour	[0-23]
 * - **min **: Minute	[0-59]
 * - **sec **: Second	[0-60] (1 leap second)
 * - **yday**: Day in year[0-365]
 * - **wday**: Day of week	[0-6]
 *
 */
sysinfo.uptimeSince = function uptimeSince() {
  var args;
  procps.sysinfo_uptimesince(function () { args = arguments; });
  return {
      year : args[0]
    , mon  : args[1]
    , mday : args[2]
    , hour : args[3]
    , min  : args[4]
    , sec  : args[5]
    , yday : args[6]
    , wday : args[7]
  }

  return { uptime: args[0], idle: args[1] };
}
