'use strict';

var procps;
try {
  procps = require('./build/Release/procps');
} catch (e) {
  // procps is not supported on this platform which is why it didn't build
  // therefore we'll supply stubs for all methods in order to not throw errors
  procps = require('./procps-stub')
}

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

/**
 * Returns various virtual memory statistics.
 *
 * Source: `/proc/vmstat`
 *
 * @name vminfo
 * @function
 * @return {Object} with the following properties:
 *
 *  - **nrDirty**          : dirty writable pages
 *  - **nrWriteback**      : pages under writeback
 *  - **nrPagecache**      : pages in pagecache -- gone in 2.5.66+ kernels
 *  - **nrPageTablePages** : pages used for pagetables
 *  - **nrReverseMaps**    : includes PageDirect
 *  - **nrMapped**         : mapped into pagetables
 *  - **nrSlab**           : in slab
 *  - **pgpgin**           : kB disk reads  (same as 1st num on /proc/stat page line)
 *  - **pgpgout**          : kB disk writes (same as 2nd num on /proc/stat page line)
 *  - **pswpin**           : swap reads     (same as 1st num on /proc/stat swap line)
 *  - **pswpout**          : swap writes    (same as 2nd num on /proc/stat swap line)
 *  - **pgalloc**          : page allocations
 *  - **pgfree**           : page freeings
 *  - **pgactivate**       : pages moved inactive -> active
 *  - **pgdeactivate**     : pages moved active -> inactive
 *  - **pgfault**          : total faults (major+minor)
 *  - **pgmajfault**       : major faults
 *  - **pgscan**           : pages scanned by page reclaim
 *  - **pgrefill**         : inspected by refill_inactive_zone
 *  - **pgsteal**          : total pages reclaimed
 *  - **kswapdSteal**      : pages reclaimed by kswapd
 *  - **pageoutrun**       : times kswapd ran page reclaim
 *  - **allocstall**       : times a page allocator ran direct reclaim
 */
sysinfo.vminfo = function () {
  var args;
  procps.sysinfo_vminfo(function () { args = arguments; });

  return [
    'nrDirty'
  , 'nrWriteback'
  , 'nrPagecache'
  , 'nrPageTablePages'
  , 'nrReverseMaps'
  , 'nrMapped'
  , 'nrSlab'
  , 'pgpgin'
  , 'pgpgout'
  , 'pswpin'
  , 'pswpout'
  , 'pgalloc'
  , 'pgfree'
  , 'pgactivate'
  , 'pgdeactivate'
  , 'pgfault'
  , 'pgmajfault'
  , 'pgscan'
  , 'pgrefill'
  , 'pgsteal'
  , 'kswapdSteal'
  , 'pageoutrun'
  , 'allocstall'
  ].reduce(function (acc, k, idx) {
      acc[k] = args[idx];
      return acc;
    }, {})
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
 * - **cpuUse**    : non-nice user cpu ticks
 * - **cpuNic**    : nice user cpu ticks
 * - **cpuSys**    : system cpu ticks
 * - **cpuIdl**    : idle cpu ticks
 * - **cpuIow**    : IO-wait cpu ticks
 * - **cpuXxx**    : IRQ cpu ticks
 * - **cpuYyy**    : softirq cpu ticks
 * - **cpuZzz**    : stolen irq ticks
 * - **pgpgin**    : pages paged in
 * - **pgpgout**   : pages paged out
 * - **pswpin**    : pages swapped in
 * - **pswpout**   : pages swapped out
 * - **intr**      : interrupts
 * - **ctxt**      : CPU context switches
 * - **running**   : processes running
 * - **blocked**   : processes blocked
 * - **btime**     : boot time
 * - **processes** : forks
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

/**
 * Returns uptime since structured into years, months, etc. for easy logging.
 * Very similar to `uptime -s` command.
 *
 * @name sysinfo::uptime
 * @function
 * @return {Object} with the following properties:
 *
 *  - **uptime**: total uptime in seconds
 *  - **idletime**: total idletime in seconds
 */
sysinfo.uptime = function uptime() {
  var args;
  procps.sysinfo_uptime(function () { args = arguments; });

  return { uptime: args[0], idle: args[1] };
}

/**
 * Returns information about since when the machine is up.
 * The result is structured into years, months, etc. for easy logging.
 *
 * Very similar to the `uptime -s` command.
 *
 * @name sysinfo::uptimeSince
 * @function
 * @return {Object} with the following properties:
 *
 * - **year**: Year	- 1900
 * - **mon** : Month	[0-11]
 * - **mday**: Day		[1-31]
 * - **hour**: Hour	[0-23]
 * - **min **: Minute	[0-59]
 * - **sec **: Second	[0-60] (1 leap second)
 * - **yday**: Day in year[0-365]
 * - **wday**: Day of week	[0-6]
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
}

/**
 * Convenience function that provides information about number and users, uptime and loadavg.
 *
 * @name sysinfo::uptimeString
 * @function
 * @param {boolean} humanReadable  if `true` only uptime is included in human readable format, otherwise all information is included.
 * @return {String} with uptime information
 */
sysinfo.uptimeString = function (humanReadable) {
  var args;
  var readable = humanReadable ? 1 : 0;
  procps.sysinfo_uptimestring(readable, function () { args = arguments; });
  return args[0];
}

/**
 * Returns load average figures giving the number of jobs in the run queue (state R) or waiting for disk I/O (state D) averaged
 * over 1, 5 and 15 minutes.
 *
 * They are the same as the load average numbers given by uptime(1) and other programs.
 *
 * Source: `/proc/loadavg`
 *
 * @name sysinfo::loadavg
 * @function
 * @return {Array<number>} three numbers representing loadaverages over 1, 5 and 15 minutes respectively
 */
sysinfo.loadavg = function () {
  var args;
  procps.sysinfo_loadavg(function () { args = Array.prototype.slice.call(arguments); });

  return args;
}

/**
 * Returns the number of digits in `PID_MAX`.
 *
 * `PID_MAX` specifies the value at which PIDs wrap around (i.e., the value in this file is one greater than the maximum PID).
 * The default value for this file, 32768, results in the same range of PIDs as on earlier kernels.
 *
 * On **32-bit platforms**, `32768` is the maximum value for pid_max.
 * On **64-bit systems**, pid_max can be set to any value up to `2^22` (`PID_MAX_LIMIT`, approximately 4 million).
 *
 * Source: `/proc/sys/kernel/pid_max`
 *
 * @name sysinfo::getPidDigits
 * @function
 * @return {number} the number of digits in `PID_MAX`
 */
sysinfo.getPidDigits = function () {
  var args;
  procps.sysinfo_getpiddigits(function () { args = arguments; });
  return args[0];
}

/**
 * Returns kernel slab allocator statistics.
 * Frequently used objects in the Linux kernel (buffer heads, inodes, dentries, * etc.)  have their own cache.
 *
 * For each slab cache, the cache name, the number of currently active objects,
 * the total number of available objects, the size of each object in bytes, the
 * number of pages with at least one active object, the total number of
 * allocated pages, and the number of pages per slab are given.
 *
 * [slabinfo man page](http://linux.die.net/man/5/slabinfo)
 *
 * Source: `/proc/slabinfo `
 *
 * ##### NOTE
 *
 * Since `/proc/slabinfo` is only accessible to root, you need to run the process with `sudo` to access slabinfo.
 * It is therefore recommended to use this only when writing a script, **please NEVER run your server as root!**
 *
 * @name sysinfo::getslabinfo
 * @function
 * @return {Array.<Object>} each with the following properties:
 *
 * - **name**: cache name
 * - **numObjs**: the total number of available objects
 * - **objsperslab**: the number of objects per slab
 * - **objsize**: the size of each object in bytes
 * - **activeObjs**: the number of currently active objects
 */
sysinfo.getslabinfo = function () {
  var args;
  procps.sysinfo_getslabinfo(function () { args = Array.prototype.slice.call(arguments); });
  return args;
}
