'use strict';

var test = require('tap').test
var sysinfo = require('../').sysinfo

function inspect(obj, depth) {
  console.error(require('util').inspect(obj, false, depth || 5, true));
}

test('\nuptime', function (t) {
  var r = sysinfo.uptime();
  t.ok(r.uptime > 0, 'greater zero uptime')
  t.ok(r.idle > 0, 'greater zero idle')
  t.end()
})

test('\nuptimeSince', function (t) {
  var r = sysinfo.uptimeSince();
  var n = new Date();

  t.equal(r.year, n.getYear() + 1900, 'up since this year')
  t.ok(r.mon <= (n.getMonth() + 1), 'up since on or before this month')
  t.ok(r.wday <= (n.getDay() + 1), 'up since on or before this weekday')
  t.ok(isFinite(r.mday), 'has mday')
  t.ok(r.yday > 0, 'has non zero yday')
  t.ok(isFinite(r.hour), 'has hour')
  t.ok(isFinite(r.min), 'has min')
  t.ok(isFinite(r.sec), 'has sec')

  t.end()
})

test('\nuptimeString', function (t) {
  var def = sysinfo.uptimeString();
  t.similar(def, /^ *(\d|:)+ up/, 'default uptime string contains uptime')
  t.similar(def, /\d user/, 'default uptime string contains user')
  t.similar(def, /load average: (\d|\.|,| )+$/, 'default uptime string contains load average')

  // FIXME: why is human readable flag not working on travis?
  if (!process.env.TRAVIS) {
    var hr = sysinfo.uptimeString(true);
    t.similar(hr, /\d+ (day|hours|minutes)/, 'human readable uptime string contains days, hours or minutes')
  }

  t.end()
})
