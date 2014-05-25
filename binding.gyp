{
  "targets": [
    {
      "target_name": "procps",
      "include_dirs": ["./deps/procps/","<!(node -e \"require('nan')\")" ],
      # VERSION numbers are picked up by procps (see procps/proc/version.c)
      "cflags": [  "-DVERSION=\"3\"", "-DSUBVERSION=\"2\"", "-DMINORVERSION=\"8\"" ],
      "sources": [
          "src/procps.cc"
        , "src/proc.cc"
        , "deps/procps/proc/alloc.c"
        , "deps/procps/proc/devname.c"
        , "deps/procps/proc/escape.c"
        , "deps/procps/proc/ksym.c"
        , "deps/procps/proc/pwcache.c"
        , "deps/procps/proc/readproc.c"
        , "deps/procps/proc/sig.c"
        , "deps/procps/proc/slab.c"
        , "deps/procps/proc/smaps.c"
        , "deps/procps/proc/sysinfo.c"
        , "deps/procps/proc/version.c"
        , "deps/procps/proc/whattime.c"
        ]
    }
  ]
}
