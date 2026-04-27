project('tdom', 'c',
  version:            '0.9.6.5',
  # Match the autotools build's scope: no explicit -std= (use the
  # compiler default), and warning_level=1 (just -Wall, no -Wextra).
  default_options:    ['warning_level=1'],
  meson_version:      '>=1.3.0',
)

deps          = []
inc           = []
pkg_sources   = []
conf          = configuration_data()
teaconf_files = []

cc = meson.get_compiler('c')
fs = import('fs')

add_project_arguments('-D_DEFAULT_SOURCE', language: 'c')

# Suppress cosmetic upstream warnings at the project level so the source
# can stay pristine (only actual bugs get patched).  Extra silencing is
# needed when certain options are on (aolserver, tdomalloc, ...).
add_project_arguments(
  cc.get_supported_arguments([
    '-Wno-unused-variable',          # static memsuite in dom.h per-TU
  ]),
  language: 'c',
)

# tdominit.c expects PACKAGE_NAME / PACKAGE_VERSION on the command line
# (they're in teaconf's config.h too, but the sources don't include it).
add_project_arguments(
  '-DPACKAGE_NAME="' + meson.project_name() + '"',
  '-DPACKAGE_VERSION="' + meson.project_version() + '"',
  language: 'c',
)

# --- Feature options → compile-time defines ---
if get_option('dtd')
  add_project_arguments('-DXML_DTD=1', language: 'c')
endif
if get_option('ns')
  add_project_arguments('-DXML_NS=1', language: 'c')
endif
if not get_option('unknown')
  add_project_arguments('-DTDOM_NO_UNKNOWN_CMD=1', language: 'c')
endif
if get_option('tdomalloc')
  pkg_sources += files('generic/domalloc.c')
else
  add_project_arguments('-DUSE_NORMAL_ALLOCATOR=1', language: 'c')
endif
if get_option('lessns')
  add_project_arguments('-DTDOM_LESS_NS=1', language: 'c')
endif
if not get_option('schema')
  add_project_arguments('-DTDOM_NO_SCHEMA=1', language: 'c')
endif

# --- Feature tests ---
if cc.has_function('memmove', prefix: '#include <string.h>')
  add_project_arguments('-DHAVE_MEMMOVE=1', language: 'c')
endif
if cc.has_function('bcopy', prefix: '#include <strings.h>')
  add_project_arguments('-DHAVE_BCOPY=1', language: 'c')
endif
if cc.has_type('uintptr_t', prefix: '#include <stdint.h>')
  add_project_arguments('-DHAVE_UINTPTR_T=1', language: 'c')
endif

# --- Optional HTML5 parsing support (libgumbo) ---
gumbo_dep = dependency('gumbo', required: get_option('html5'))
if gumbo_dep.found()
  deps += gumbo_dep
  add_project_arguments('-DTDOM_HAVE_GUMBO=1', language: 'c')
endif

# --- Optional AOLserver integration ---
# generic/aolstub.cpp is #included from tdominit.c when NS_AOLSERVER is
# set, so it doesn't enter pkg_sources as a separate TU.  The include
# path is deferred and appended after our own source dirs so that any
# stale tdom/expat/schema headers in the AOLserver install don't shadow
# our own (see below, after subdir('generic')).
aol_dir = get_option('aolserver')
if aol_dir != ''
  if not fs.is_file(aol_dir / 'include/ns.h')
    error('aolserver="' + aol_dir + '" but ' + aol_dir + '/include/ns.h not found')
  endif
  if get_option('tdomalloc')
    error('aolserver and tdomalloc are mutually exclusive: AOLserver mode forces USE_NORMAL_ALLOCATOR, which replaces tDOM\'s block allocator with macros and collides with the domalloc.c definitions')
  endif
  inc += include_directories(aol_dir / 'include')
  add_project_arguments('-DNS_AOLSERVER=1',         language: 'c')
  add_project_arguments('-DUSE_NORMAL_ALLOCATOR=1', language: 'c')
  # ns.h pulls in zlib.h which uses off64_t from <sys/types.h>; glibc
  # only exposes that when _LARGEFILE64_SOURCE is set.
  add_project_arguments('-D_LARGEFILE64_SOURCE=1',  language: 'c')
  # On Darwin the autotools also link -lnsd -lnsthread; elsewhere the
  # AOLserver runtime provides those symbols at module-load time.
  if host_machine.system() == 'darwin'
    deps += declare_dependency(
      link_args: ['-L' + aol_dir / 'lib', '-lnsd', '-lnsthread'],
    )
  endif
  # Ns_TclInitInterps (AOLserver 4.x)
  add_project_arguments(cc.get_supported_arguments(['-Wno-deprecated-declarations']), language: 'c')
endif

# Tcl 9 is always threaded, and Tcl 8.6 has been threaded-by-default for
# years; tdom's dom attachDocument / detachDocument and related features
# depend on TCL_THREADS being set.  USE_THREAD_ALLOC matches what TEA's
# configure emits on threaded Tcl builds.
if host_machine.system() != 'windows'
  add_project_arguments(
    '-DTCL_THREADS=1',
    '-DUSE_THREAD_ALLOC=1',
    '-D_REENTRANT=1',
    '-D_THREAD_SAFE=1',
    language: 'c',
  )
  deps += dependency('threads')
else
  add_project_arguments('-DTCL_THREADS=1', language: 'c')
endif

# --- Teabase init (provides tcl_dep, tclsh, build_tclsh, pkg_dir, teaconf) ---
subdir('teabase/init')

# --- expat dependency ---
# Use the system expat if available, otherwise fall back to the meson
# wrap (downloads the upstream release and builds it as a subproject).
# The bundled expat/ sources in the repo are kept for the autotools
# build path; the meson build ignores them.  When the fallback kicks in,
# build expat statically so it folds into libtcl9tdom.so — no runtime
# libexpat dependency on the installed tdom.
deps += dependency('expat',
  version:          '>=2.6.4',
  allow_fallback:   true,
  default_options:  ['default_library=static'],
)

subdir('generic')

# --- versionhash.h (generated from manifest.uuid / git rev, consumed by tcldom.c) ---
if fs.exists('manifest.uuid')
  versionhash = fs.read('manifest.uuid').strip()
else
  versionhash = run_command(
    find_program('git', native: true, required: true), 'rev-parse', 'HEAD',
    capture:  true,
    check:    true,
  ).stdout().strip()
endif

versionhash_conf = configuration_data()
versionhash_conf.set_quoted('FOSSIL_HASH', versionhash)
configure_file(
  output:        'versionhash.h',
  configuration: versionhash_conf,
)
inc += include_directories('.')

# --- Install runtime Tcl sources ---
teaconf_files += files('lib/tdom.tcl')

# --- Teabase main (builds shared_module, sets up tests, devenv) ---
subdir('teabase')

# --- Optional 'big tclsh' with tDOM statically linked ---
# Mirrors the autotools Makefile's $(TDOMSHELL) rule: not built by
# default, not installed.  Build with:  meson compile -C <builddir> tcldomsh
# -UUSE_TCL_STUBS wins over the project-wide -DUSE_TCL_STUBS=1 because
# per-target c_args come after project args on the compile line, so tDOM
# here links directly against libtcl instead of going through the stubs
# table.
executable('tcldomsh',
  'unix/tclAppInit.c',
  pkg_sources,
  c_args:               ['-UUSE_TCL_STUBS'],
  include_directories:  inc,
  # `deps` provides tclstub / gumbo / threads / generated stubs sources;
  # the static flavour of the Tcl dep adds -ltcl9.0 plus the Libs.private
  # transitive closure (-lm -ldl -ltommath -lz) that libtcl9.0 references.
  dependencies:         deps + [dependency('tcl')],
  # Tcl isn't in the default dynamic linker search path when installed
  # under /opt; bake an rpath so the unistalled binary can still run.
  build_rpath:          tcl_dep.get_variable('libdir'),
  build_by_default:     false,
  install:              false,
)

# --- Install committed manpages (section n — Tcl convention) ---
# Matches autotools `install-doc`: ship the pre-generated `.n` files.
# Regeneration from the TMML .xml sources is a manual developer step
# (see doc/README); upstream doesn't automate it and dist tarballs ship
# the committed `.n` files as-is.  `install_man` rejects the `.n`
# extension, so install_data with the explicit section-n path.
install_data(
  'doc/dom.n',
  'doc/domDoc.n',
  'doc/domNode.n',
  'doc/expat.n',
  'doc/expatapi.n',
  'doc/pullparser.n',
  'doc/schema.n',
  'doc/tdomcmd.n',
  'doc/tnc.n',
  'doc/xpathFunc.n',
  install_dir: get_option('mandir') / 'mann',
)

# --- Build and install stubs library for dependent extensions ---
stubs_lib = static_library(meson.project_name() + 'stub',
  'generic/tdomStubLib.c',
  dependencies:         deps,
  include_directories:  inc,
  install:              true,
  install_dir:          pkg_dir,
)

# --- Relocatable pkg-config file for downstream extensions ---
pc_conf = configuration_data()
pc_conf.set('PACKAGE_NAME',     meson.project_name())
pc_conf.set('PACKAGE_VERSION',  meson.project_version())
pc_conf.set('PKG_DIR_BASENAME', fs.name(pkg_dir))
pc_conf.set('STUBLIB',          stubs_lib.name())

configure_file(
  input:          'tdom.pc.in',
  output:         meson.project_name() + '.pc',
  configuration:  pc_conf,
  install_dir:    tcl_dep.get_variable('libdir') / 'pkgconfig',
)

# --- Release tarball via `meson dist` ---
# tools/dist_fixup.sh rewrites the distributed manifest.uuid with the
# current HEAD hash, since git archive would otherwise export whatever
# was committed to manifest.uuid last (potentially stale).
meson.add_dist_script('tools/dist_fixup.sh')
