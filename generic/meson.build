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

# Stubs are generated and the stubs library built by the factored-out
# teabase mechanism — see `tea_genstubs` in the top-level meson.build.
inc  += include_directories('.')
