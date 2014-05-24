{
  "targets": [
    {
      "target_name": "procjs",
      # silence warnings caused by procps
      "cflags": [ "-Wno-unused-but-set-variable", "-Wno-char-subscripts", "-Wno-unused-function", "-Wno-format", "-Wno-comment", "-Wno-sign-compare", "-Wno-padded" ],
#"libraries": ["/home/vagrant/dev/do/procjs/deps/procps/proc/libproc.a"],
      "include_dirs": ["./deps/procps/"],
      # VERSION numbers are picked up by procps (see procps/proc/version.c)
      "cflags": [  "-DVERSION=\"3\"", "-DSUBVERSION=\"2\"", "-DMINORVERSION=\"8\"" ],
      "sources": [
          "src/procjs.cc"
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
