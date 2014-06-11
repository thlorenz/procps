{
  'targets': [
    {
      'conditions': [
        [ 'OS != "linux"', {
          'target_name': 'procps_only_supported_on_linux',
        } ], 
        [ 'OS == "linux"', { 
          'target_name': 'procps',
          'sources': [
              'src/procps.cc'
            , 'src/proc.cc'
            , 'src/diskstat.cc'
            , 'src/partitionstat.cc'
            , 'src/slabcache.cc'
            , 'deps/procps/proc/alloc.c'
            , 'deps/procps/proc/devname.c'
            , 'deps/procps/proc/escape.c'
            , 'deps/procps/proc/ksym.c'
            , 'deps/procps/proc/pwcache.c'
            , 'deps/procps/proc/readproc.c'
            , 'deps/procps/proc/sig.c'
            , 'deps/procps/proc/slab.c'
            , 'deps/procps/proc/sysinfo.c'
            , 'deps/procps/proc/version.c'
            , 'deps/procps/proc/whattime.c'
            ],
          'include_dirs': [
              './deps/procps/include/'
            , './deps/procps/'
            , '<!(node -e "require(\'nan\')")' 
            ],
          # VERSION numbers are picked up by procps (see procps/proc/version.c)
          # TODO: Why does the C++ compiler pick up the C flags and complain about them ???
            'cflags': [ 
                '-DPACKAGE_NAME=\"procps\"'
              , '-DPACKAGE_VERSION=\"3.3.9\"'
              , '-DBUILD_WITH_WHINE=1'
            ],
            'cflags!'   : [ '-fno-exceptions', '-fno-tree-vrp', '-fno-tree-sink' ],

            'cflags_c'  : [ '--std=gnu99', '-Wno-string-plus-int', '-Wno-sign-compare' ],
            'cflags_cc' : [ '-fexceptions', '-frtti' ],
        } ],
      ]
    }
  ]
}
