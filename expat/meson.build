pkg_sources += files(
  'xmlparse.c',
  'xmlrole.c',
  'xmltok.c',
)
inc += include_directories('.')

add_project_arguments('-DXML_CONTEXT_BYTES=0',  language: 'c')
add_project_arguments('-DXML_GE=1',             language: 'c')
if host_machine.system() == 'windows'
  add_project_arguments('-DXML_STATIC=1',       language: 'c')
  add_project_arguments('-DXML_LARGE_SIZE=1',   language: 'c')
endif

# --- expat entropy source ---
# Mirrors tdom.m4's TDOM_EXPAT_ENTROPY auto branch: two *independent*
# fallback chains (arc4random_buf → arc4random; getrandom →
# syscall_getrandom), then XML_DEV_URANDOM and XML_POOR_ENTROPY are
# always also defined in the auto case.

have_arc4 = false
if get_option('entropy') in ['auto', 'arc4random_buf']
  if cc.links('''
    #include <stdlib.h>
    int main(void) { arc4random_buf(0, 0); return 0; }
  ''', name: 'arc4random_buf')
    add_project_arguments('-DHAVE_ARC4RANDOM_BUF=1', language: 'c')
    have_arc4 = true
  endif
endif
if not have_arc4 and get_option('entropy') in ['auto', 'arc4random']
  if cc.links('''
    #include <stdlib.h>
    int main(void) { arc4random(); return 0; }
  ''', name: 'arc4random')
    add_project_arguments('-DHAVE_ARC4RANDOM=1', language: 'c')
    have_arc4 = true
  endif
endif

have_grnd = false
if get_option('entropy') in ['auto', 'getrandom']
  if cc.links('''
    #include <stdlib.h>
    #include <sys/random.h>
    int main(void) { return getrandom(0, 0, 0); }
  ''', name: 'getrandom')
    add_project_arguments('-DHAVE_GETRANDOM=1', language: 'c')
    have_grnd = true
  endif
endif
if not have_grnd and get_option('entropy') in ['auto', 'syscall_getrandom']
  if cc.links('''
    #include <unistd.h>
    #include <sys/syscall.h>
    int main(void) { syscall(SYS_getrandom, 0, 0, 0); return 0; }
  ''', name: 'syscall_getrandom')
    add_project_arguments('-DHAVE_SYSCALL_GETRANDOM=1', language: 'c')
    have_grnd = true
  endif
endif

if get_option('entropy') in ['auto', 'dev_urandom']
  add_project_arguments('-DXML_DEV_URANDOM=1', language: 'c')
endif

if get_option('entropy') in ['auto', 'none'] or not (have_arc4 or have_grnd)
  add_project_arguments('-DXML_POOR_ENTROPY=1', language: 'c')
endif

# --- Install the public headers alongside the shared library in pkg_dir ---
install_headers(
  'expat_external.h',
  'expat.h',
  install_dir: pkg_dir,
)
