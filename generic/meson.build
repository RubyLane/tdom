pkg_sources += files(
  'datatypes.c',
  'dom.c',
  'domhtml5.c',
  'domhtml.c',
  'domjson.c',
  'domlock.c',
  'domxpath.c',
  'domxslt.c',
  'nodecmd.c',
  'schema.c',
  'tcldom.c',
  'tclexpat.c',
  'tclpull.c',
  'tdominit.c',
  'xmlsimple.c',
)

install_headers(
  'domalloc.h',
  'dom.h',
  'domxpath.h',
  'nametab.h',
  'schema.h',
  'tcldom.h',
  'tclexpat.h',
  'tdom.h',
  install_dir: pkg_dir,
)

# --- Stubs generation ---
genstubs = custom_target('genstubs',
  command: [
    build_tclsh,
    meson.project_source_root() / 'teabase/tools/genStubs.tcl',
    meson.current_build_dir(),
    files('tdom.decls'),
  ],
  output:       [ 'tdomStubInit.c',  'tdomDecls.h' ],
  install_dir:  [ false,             pkg_dir       ],
  install:      true,
)

deps += declare_dependency(sources: genstubs)
inc  += include_directories('.')
