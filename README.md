# procps [![build status](https://secure.travis-ci.org/thlorenz/procps.png?branch=master)](http://travis-ci.org/thlorenz/procps)

Node.js bindings for [procps](http://procps.sourceforge.net/), a library that provides information about processes using
the `/proc` filesystem, using the [better maintained fork](https://gitorious.org/procps/procps/).

## Supported Platforms

Unixes with a `/proc` directory only. Tested on `arch linux` and `ubuntu`.

## Installation

    npm install procps

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**  *generated with [DocToc](http://doctoc.herokuapp.com/)*

- [Example](#example)
- [API](#api)
    - [readproctab::flags](#readproctabflags)
    - [readproctab::flagsFillAll](#readproctabflagsfillall)
    - [readproctab(flags_) → {Array.<Object>}](#readproctabflags_-→-arrayobject)
    - [sysinfo::getdiskstat() → {Object}](#sysinfogetdiskstat-→-object)
    - [sysinfo::getstat() → {Object}](#sysinfogetstat-→-object)
    - [sysinfo::meminfo(unit) → {Object}](#sysinfomeminfounit-→-object)
- [Caveats](#caveats)
- [LICENSE](#license)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Example

```js
var procps = require('procps');

var proctab = procps.readproctab();
var bycommand = proctab.reduce(function (acc, p) {
  acc[p.cmd] = p;
  return acc;
}, {});

console.log(bycommand.node)
```

```
{ cutime: 0,
  majFlt: 0,
  vmSize: 661152,
  resident: 0,
  startCode: 4194304,
  rgroup: 'kermit',
  egid: 1000,
  pgrp: 13690,
  minFlt: 3199,
  startTime: 3344937,
  suser: 'kermit',
  fgid: 1000,
  processor: 0,
  startStack: 76310960,
  sgroup: 'kermit',
  cmajFlt: 0,
  flags: 4202496,
  rss: 2431,
  tid: 13834,
  ...
  environ:
   [ 'LC_PAPER=en_US.UTF-8',
     ...
     'npm_node_execpath=/usr/bin/node' ],
  nlwp: 2,
  kstkEsp: 76309144,
  fuser: 'kermit',
  cmdline: [ 'node', 'example/readproctab.js' ],
  sigcatch: '0000000180014202',
  utime: 2,
  vmRss: 9724,
  wchan: 4294967295,
  euid: 1000,
  sigignore: '0000000000001000',
  share: 0 }
```

## API

*Unofficial* [procps documentation](http://fossies.org/dox/procps-3.2.8/).

So far `readproctab` has been implemented, but lots more to come, i.e. `meminfo`.

<!-- START docme generated API please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN docme TO UPDATE -->

<div>
<div class="jsdoc-githubify">
<section>
<article>
<div class="container-overview">
<dl class="details">
</dl>
</div>
<dl>
<dt>
<h4 class="name" id="readproctab::flags"><span class="type-signature"></span>readproctab::flags<span class="type-signature"></span></h4>
</dt>
<dd>
<div class="description">
<p>A hashtable of all readproc flags.
Use these in order to fill/loose specific process properties.</p>
</div>
<dl class="details">
<dt class="tag-source">Source:</dt>
<dd class="tag-source"><ul class="dummy">
<li>
<a href="https://github.com/thlorenz/procps/blob/sysinfo/index.js">index.js</a>
<span>, </span>
<a href="https://github.com/thlorenz/procps/blob/sysinfo/index.js#L56">lineno 56</a>
</li>
</ul></dd>
</dl>
</dd>
<dt>
<h4 class="name" id="readproctab::flagsFillAll"><span class="type-signature"></span>readproctab::flagsFillAll<span class="type-signature"></span></h4>
</dt>
<dd>
<div class="description">
<p>The flags used by default which cause readproc to fill all properties of each process.
Use them as a starting point to turn properties off selectively, i.e.:</p>
<pre><code class="lang-js">var flags = readproctab.flagsFillAll ^ readproctab.flags.PROC_FILLENV ^ readproctab.flags.PROC_FILLUSR;</code></pre>
</div>
<dl class="details">
<dt class="tag-source">Source:</dt>
<dd class="tag-source"><ul class="dummy">
<li>
<a href="https://github.com/thlorenz/procps/blob/sysinfo/index.js">index.js</a>
<span>, </span>
<a href="https://github.com/thlorenz/procps/blob/sysinfo/index.js#L64">lineno 64</a>
</li>
</ul></dd>
</dl>
</dd>
</dl>
<dl>
<dt>
<h4 class="name" id="readproctab"><span class="type-signature"></span>readproctab<span class="signature">(<span class="optional">flags_</span>)</span><span class="type-signature"> &rarr; {Array.&lt;Object>}</span></h4>
</dt>
<dd>
<div class="description">
<p>Calls underlying readproctab and returns results with the following ajustments:</p>
<ul>
<li>underscore_names are camel cased</li>
<li>Int64 values are converted to Int32 values</li>
</ul>
</div>
<h5>Parameters:</h5>
<table class="params">
<thead>
<tr>
<th>Name</th>
<th>Type</th>
<th>Argument</th>
<th class="last">Description</th>
</tr>
</thead>
<tbody>
<tr>
<td class="name"><code>flags_</code></td>
<td class="type">
<span class="param-type">number</span>
</td>
<td class="attributes">
&lt;optional><br>
</td>
<td class="description last"><p>flags passed to readproc, allow filling specific process properties only instead of all of them which is the default</p></td>
</tr>
</tbody>
</table>
<dl class="details">
<dt class="tag-source">Source:</dt>
<dd class="tag-source"><ul class="dummy">
<li>
<a href="https://github.com/thlorenz/procps/blob/sysinfo/index.js">index.js</a>
<span>, </span>
<a href="https://github.com/thlorenz/procps/blob/sysinfo/index.js#L35">lineno 35</a>
</li>
</ul></dd>
</dl>
<h5>Returns:</h5>
<div class="param-desc">
<p>information about all processes running on the system</p>
</div>
<dl>
<dt>
Type
</dt>
<dd>
<span class="param-type">Array.&lt;Object></span>
</dd>
</dl>
</dd>
<dt>
<h4 class="name" id="sysinfo::getdiskstat"><span class="type-signature"></span>sysinfo::getdiskstat<span class="signature">()</span><span class="type-signature"> &rarr; {Object}</span></h4>
</dt>
<dd>
<div class="description">
<p>Gets statistics about disks/devices and partitions on the machine.</p>
<h5>Example DiskStats</h5>
<pre><code class="lang-js">{ disks:
[ { diskName: 'sda',
writes: 51770,
weightedMilliSpentIO: 121633,
reads: 14706,
partitions: 2,
milliWriting: 102280,
milliSpentIO: 24633,
milliReading: 19366,
mergedWrites: 131130,
mergedReads: 3164,
inprogressIO: 0,
writtenSectors: 1554100,
readsSectors: 486100 },
{ diskName: 'loop0',
...
partitions:
[ { partitionName: 'sda1',
requestedWrites: 1554100,
writes: 51693,
reads: 14553,
parentDisk: 0,
readsSectors: 483762 },
{ partitionName: 'sda2',
...
]}</code></pre>
</div>
<dl class="details">
<dt class="tag-source">Source:</dt>
<dd class="tag-source"><ul class="dummy">
<li>
<a href="https://github.com/thlorenz/procps/blob/sysinfo/index.js">index.js</a>
<span>, </span>
<a href="https://github.com/thlorenz/procps/blob/sysinfo/index.js#L212">lineno 212</a>
</li>
</ul></dd>
</dl>
<h5>Returns:</h5>
<div class="param-desc">
<p>with <code>disks</code> array and <code>partitions</code> array</p>
</div>
<dl>
<dt>
Type
</dt>
<dd>
<span class="param-type">Object</span>
</dd>
</dl>
</dd>
<dt>
<h4 class="name" id="sysinfo::getstat"><span class="type-signature"></span>sysinfo::getstat<span class="signature">()</span><span class="type-signature"> &rarr; {Object}</span></h4>
</dt>
<dd>
<div class="description">
<p>Gets statistics about cpu, process and memory usage.
<code>procps.getstat</code> used by various <code>vmstat</code> functions.</p>
</div>
<dl class="details">
<dt class="tag-source">Source:</dt>
<dd class="tag-source"><ul class="dummy">
<li>
<a href="https://github.com/thlorenz/procps/blob/sysinfo/index.js">index.js</a>
<span>, </span>
<a href="https://github.com/thlorenz/procps/blob/sysinfo/index.js#L145">lineno 145</a>
</li>
</ul></dd>
</dl>
<h5>Returns:</h5>
<div class="param-desc">
<p>with the following properties:</p>
<ul>
<li><strong>cpuUse</strong>:    non-nice user cpu ticks</li>
<li><strong>cpuNic</strong>:    nice user cpu ticks</li>
<li><strong>cpuSys</strong>:    system cpu ticks</li>
<li><strong>cpuIdl</strong>:    idle cpu ticks</li>
<li><strong>cpuIow</strong>:    IO-wait cpu ticks</li>
<li><strong>cpuXxx</strong>:    IRQ cpu ticks</li>
<li><strong>cpuYyy</strong>:    softirq cpu ticks</li>
<li><strong>cpuZzz</strong>:    stolen irq ticks</li>
<li><strong>pgpgin</strong>:    pages paged in</li>
<li><strong>pgpgout</strong>:   pages paged out</li>
<li><strong>pswpin</strong>:    pages swapped in</li>
<li><strong>pswpout</strong>:   pages swapped out</li>
<li><strong>intr</strong>:      interrupts</li>
<li><strong>ctxt</strong>:      CPU context switches</li>
<li><strong>running</strong>:   processes running</li>
<li><strong>blocked</strong>:   processes blocked</li>
<li><strong>btime</strong>:     boot time</li>
<li><strong>processes</strong>: forks</li>
</ul>
</div>
<dl>
<dt>
Type
</dt>
<dd>
<span class="param-type">Object</span>
</dd>
</dl>
</dd>
<dt>
<h4 class="name" id="sysinfo::meminfo"><span class="type-signature"></span>sysinfo::meminfo<span class="signature">(<span class="optional">unit</span>)</span><span class="type-signature"> &rarr; {Object}</span></h4>
</dt>
<dd>
<div class="description">
<p>A hybrid of <code>procps.meminfo</code> and <code>free</code>.</p>
</div>
<h5>Parameters:</h5>
<table class="params">
<thead>
<tr>
<th>Name</th>
<th>Type</th>
<th>Argument</th>
<th class="last">Description</th>
</tr>
</thead>
<tbody>
<tr>
<td class="name"><code>unit</code></td>
<td class="type">
<span class="param-type">string</span>
</td>
<td class="attributes">
&lt;optional><br>
</td>
<td class="description last"><p><code>'b'|'k'|'m'|'g'</code> to return usage in Bytes|KB|MB|GB respectively</p></td>
</tr>
</tbody>
</table>
<dl class="details">
<dt class="tag-source">Source:</dt>
<dd class="tag-source"><ul class="dummy">
<li>
<a href="https://github.com/thlorenz/procps/blob/sysinfo/index.js">index.js</a>
<span>, </span>
<a href="https://github.com/thlorenz/procps/blob/sysinfo/index.js#L86">lineno 86</a>
</li>
</ul></dd>
</dl>
<h5>Returns:</h5>
<div class="param-desc">
<p>with properties indicating memory usage, like <code>mainTotal</code></p>
</div>
<dl>
<dt>
Type
</dt>
<dd>
<span class="param-type">Object</span>
</dd>
</dl>
</dd>
</dl>
</article>
</section>
</div>

*generated with [docme](https://github.com/thlorenz/docme)*
</div>
<!-- END docme generated API please keep comment here to allow auto update -->

## LICENSE

MIT
