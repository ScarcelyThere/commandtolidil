project( 'dj3600cmdfilter', 'cpp', version: '0.9' )

# This is specifically built by my custom Meson build in HP's regular hplip source
#  tree. Should you be using, say, your distribution's version, there's a build
#  option for that later.
hpmud = dependency( 'hpmud', required: false )
cups  = dependency( 'cups' )

filter_install_dir = cups.get_variable( 'cups_serverbin' ) / 'filter'

hpmudsource = 'HpmudBackend.cpp'
hpmuddefine = 'BUILD_HPMUD'

sources = [ 'Pen.cpp', 'DeskJet3600.cpp',
            'dj3600command.cpp', 'USBBackend.cpp' ]

args     = [ ]
linkargs = [ ]

if hpmud.found()
  message( 'Custom hpmud found, enabling it' )
  deps = [ cups, hpmud ]
  sources = sources + hpmudsource
  args = '-D' + hpmuddefine
else
  message( 'Custom hpmud not found, falling back to your supplied option' )
  if get_option( 'hpbackend' ).enabled( )
    message( ' ...which is enabled' )
    linkargs = [ '-lhpmud' ]
    sources = sources + hpmudsource
    args = '-D' + hpmuddefine
  else
    message( ' ...which is NOT enabled' )
  endif

  deps = [ cups ]
endif

dj3600cmdfilter = executable( 'dj3600command', sources,
                              install: true,
                              cpp_args: args,
                              link_args: linkargs,
                              install_dir: filter_install_dir,
                              dependencies: deps )

pentest = executable( 'pentest', [ 'USBBackend.cpp', 'Test.cpp', 'Pen.cpp',
                                   'DeskJet3600.cpp' ],
                      install: false,
                      dependencies: cups )
