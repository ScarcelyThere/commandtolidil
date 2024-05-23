# Meson build for commandtolidil
# This is free software; you can redistribute it and/or modify it under the terms
# of the GNU General Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any other version.
#
# This is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# commandtolidil. If not, see <https://www.gnu.org/licenses/>.

project( 'commandtolidil', 'cpp', version: '1.0.0' )

# This is specifically built by my custom Meson build in HP's regular hplip source
#  tree. Should you be using, say, your distribution's version, there's a build
#  option for that later.
hpmud = dependency( 'hpmud', required: false )
cups  = dependency( 'cups' )

filter_install_dir = cups.get_variable( 'cups_serverbin' ) / 'filter'

hpmudsource = 'HpmudBackend.cpp'
hpmuddefine = 'BUILD_HPMUD'

sources = [ 'Pen.cpp', 'DeskJet3600.cpp', 'Backend.cpp',
            'commandtolidil.cpp', 'USBBackend.cpp',
            'CUPS_Utilities.cpp' ]

args     = [ ]

if hpmud.found()
  message( 'Custom HPMUD found, enabling it (thanks for using hplip_meson!)' )
  deps = [ cups, hpmud ]
  sources = sources + hpmudsource
  args = '-D' + hpmuddefine
else
  deps = [ cups ]
endif

commandtolidil = executable( 'commandtolidil', sources,
                             install: true,
                             cpp_args: args,
                             install_dir: filter_install_dir,
                             dependencies: deps )

if get_option( 'tests' ).enabled( )
  commandtest = executable( 'commandtest', [ 'USBBackend.cpp', 'Test.cpp', 'Pen.cpp',
                                             'DeskJet3600.cpp', 'TestBackend.cpp',
                                             'Backend.cpp', 'CUPS_Utilities.cpp' ],
                            install: false,
                            dependencies: cups,
                            cpp_args: '-DTESTING' )
endif
