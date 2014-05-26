# procps [![build status](https://secure.travis-ci.org/thlorenz/procps.png?branch=master)](http://travis-ci.org/thlorenz/procps)

Node.js bindings for [procps](http://procps.sourceforge.net/), a library that provides information about processes using the `/proc` filesystem.  

## Supported Platforms

Unixes with a `/proc` directory only. Tested on `arch linux` and `ubuntu`.

At this point Node.js versions `0.8, 0.10` are supported, support for `0.11` is in the works.

## Installation

    npm install procps

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
  ...
  pgrp: 13690,
  minFlt: 3199,
  startTime: 3344937,
  suser: 'kermit',
  fgid: 1000,
  processor: 0,
  nice: 0,
  startStack: 76310960,
  fuid: 1000,
  vmStack: 140,
  euser: 'kermit',
  size: 0,
  sgroup: 'kermit',
  cmajFlt: 0,
  flags: 4202496,
  rss: 2431,
  tid: 13834,
  ...
  environ:
   [ 'LC_PAPER=en_US.UTF-8',
     ...
     'npm_package_scripts_build=CC=gcc CXX=gcc node-gyp rebuild',
     'npm_package_scripts_start=npm run build',
     'npm_package_keywords_2=memory',
     'npm_node_execpath=/usr/bin/node' ],
  nlwp: 2,
  kstkEsp: 76309144,
  alarm: 0,
  fuser: 'kermit',
  cmdline: [ 'node', 'example/readproctab.js' ],
  sigcatch: '0000000180014202',
  blocked: '0000000000000000',
  utime: 2,
  vmRss: 9724,
  vmLock: 0,
  wchan: 4294967295,
  dt: 0,
  priority: 20,
  stime: 1,
  suid: 1000,
  rgid: 1000,
  euid: 1000,
  sigignore: '0000000000001000',
  share: 0 }
```

## API

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
<a href="https://github.com/thlorenz/procps/blob/master/index.js">index.js</a>
<span>, </span>
<a href="https://github.com/thlorenz/procps/blob/master/index.js#L52">lineno 52</a>
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
<a href="https://github.com/thlorenz/procps/blob/master/index.js">index.js</a>
<span>, </span>
<a href="https://github.com/thlorenz/procps/blob/master/index.js#L60">lineno 60</a>
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
<a href="https://github.com/thlorenz/procps/blob/master/index.js">index.js</a>
<span>, </span>
<a href="https://github.com/thlorenz/procps/blob/master/index.js#L31">lineno 31</a>
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
</dl>
</article>
</section>
</div>

*generated with [docme](https://github.com/thlorenz/docme)*
</div>
<!-- END docme generated API please keep comment here to allow auto update -->

## LICENSE

MIT
