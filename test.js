/*jshint asi: true, laxbreak: true, laxcomma: true */
/*globals Procjs */

var logs = [];

var flags = {
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

function logProcs(pjs) {

  var args;
  // passing result as args array instead of v8::Array return value
  pjs.procs(function () { args = arguments; });

  var s;
  for (var i  = 0; i < args.length; i++) {
    var ps = args[i];
    s =  '\n' + ps.ppid + ' tid: ' + ps.tid + ' cmd: ' + ps.cmd;
    if (ps.cmdline && ps.cmdline.length) {
      s += ' cmdline: [ ';
      for (var j = 0; j < ps.cmdline.length; j++) s += ps.cmdline[j] + ' ';
      s += ' ]';
    }

    if (ps.environ && ps.environ.length) {
      s += '\n  environ: [ ';
      for (var k = 0; k < ps.environ.length; k++) s += ps.environ[k] + ' ';
      s += '\n]';
    }

    s += '\n  ' +
      ' pcpu: '   + ps.pcpu    +
      ' state: '  + ps.state;
    s += '\n  ' +
      ' utime: '  + ps.utime   +
      ' stime: '  + ps.stime;
    s += '\n  ' +
      ' cutime: '     + ps.cutime      +
      ' cstime: '     + ps.cstime      +
      ' startTime: '  + ps.startTime;
    s += '\n  ' +
      ' signal: '     + ps.signal     +
      ' blocked: '    + ps.blocked    +
      ' sigignore: '  + ps.sigignore  +
      ' sigcatch: '   + ps.sigcatch   +
      ' _sigpnd: '    + ps._sigpnd;
    s += '\n  ' +
      ' startCode: '   + ps.startCode   +
      ' endCode: '     + ps.endCode     +
      ' startStack: '  + ps.startStack  +
      ' kstkEsp: '     + ps.kstkEsp     +
      ' kstkEip: '     + ps.kstkEip     +
      ' wchan: '       + ps.wchan;
    s += '\n  ' +
      ' priority: '  + ps.priority  +
      ' nice: '      + ps.nice      +
      ' rss: '       + ps.rss       +
      ' alarm: '     + ps.alarm     +
      ' size: '      + ps.size      +
      ' resident: '  + ps.resident  +
      ' share: '     + ps.share     +
      ' trs: '       + ps.trs       +
      ' lrs: '       + ps.lrs       +
      ' drs: '       + ps.drs       +
      ' dt: '        + ps.dt;
    s += '\n  ' +
      ' vmSize: '   + ps.vmSize   +
      ' vmLock: '   + ps.vmLock   +
      ' vmRss: '    + ps.vmRss    +
      ' vmData: '   + ps.vmData   +
      ' vmStack: '  + ps.vmStack  +
      ' vmExe: '    + ps.vmExe    +
      ' vmLib: '    + ps.vmLib;
    s += '\n  ' +
      ' rtprio: ' + ps.rtprio +
      ' sched: ' + ps.sched +
      ' vsize: ' + ps.vsize +
      ' rssRlim: ' + ps.rssRlim +
      ' flags: ' + ps.flags;
    s += '\n  ' +
      ' minFlt: ' + ps.minFlt +
      ' majFlt: ' + ps.majFlt +
      ' cminFlt: ' + ps.cminFlt +
      ' cmajFlt: ' + ps.cmajFlt;
    s += '\n  ' +
      ' euser: ' + ps.euser +
      ' ruser: ' + ps.ruser +
      ' suser: ' + ps.suser +
      ' fuser: ' + ps.fuser;
    s += '\n  ' +
      ' rgroup: ' + ps.rgroup +
      ' egroup: ' + ps.egroup +
      ' sgroup: ' + ps.sgroup +
      ' fgroup: ' + ps.fgroup;
    s += '\n  ' +
      ' pgrp: ' + ps.pgrp +
      ' session: ' + ps.session +
      ' nlwp: ' + ps.nlwp +
      ' tgid: ' + ps.tgid +
      ' tty: ' + ps.tty;
    s += '\n  ' +
      ' euid: ' + ps.euid +
      ' egid: ' + ps.egid +
      ' ruid: ' + ps.ruid +
      ' rgid: ' + ps.rgid +
      ' suid: ' + ps.suid +
      ' sgid: ' + ps.sgid +
      ' fuid: ' + ps.fuid +
      ' fgid: ' + ps.fgid;
    s += '\n  ' +
      ' euid: ' + ps.euid +
      ' tpgid: ' + ps.tpgid +
      ' exitSignal: ' + ps.exitSignal +
      ' processor: ' + ps.processor;
    logs.push(s);
  }
}

var defaultFlags = 0
  | flags.PROC_FILLMEM
  | flags.PROC_FILLCOM
  | flags.PROC_FILLENV
  | flags.PROC_FILLUSR
  | flags.PROC_FILLGRP
  | flags.PROC_FILLSTATUS
  | flags.PROC_FILLSTAT
  | flags.PROC_FILLWCHAN
  | flags.PROC_FILLARG
  | flags.PROC_LOOSE_TASKS
  ;

var pjs = new Procjs(defaultFlags);
//var pjs = new Procjs(flags.PROC_FILLMEM);
logProcs(pjs);

(function() { return logs.join('\n'); })();
