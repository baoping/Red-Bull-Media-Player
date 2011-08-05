Red Bull Media Player
=====================

## Requirements

The Red Bull Media Player is built using the following frameworks and tools:

- Qt 4.5.2 (http://qt.nokia.com/)
- Qt Animation Framework v2.3 (http://qt.nokia.com/)
- VLC 1.1.0-rc2 (git://git.videolan.org/vlc/vlc-1.1.git  )
- TagLib 1.6.1 (http://developer.kde.org/~wheeler/taglib.html)
- log4qt 0.3 (http://log4qt.sourceforge.net/)
- make/qmake/ruby/rake


## Building

The sources should build without modification under the following systems:

- Windows Vista/Windows 7
- Ubuntu 9.10
- Mac OSX 10.6

Porting to other platforms/os versions is mainly a matter of making VLC build and run. 
Due to the design, the Player is delivered as a self-extracting binary package that runs without modifications on the host outside its location, all dependencies (Qt etc.) need to be packaged correctly and the runtime environment must be set up to use them (e.g. LD\_LIBRARY\_PATH on Linux)

__Note:__ _On Mac OS the application is a simple application bundle, so packing/extracting is not required_

The build system is based on Qt's qmake and wrapped by rake and make scripts.

To speed up building the build system uses precompiled versions of several dependencies that are automatically downloaded and extracted the first time the build is run. The precompiled binaries are unmodificated builds done on our development machines.

### Windows


To compile, package and run the tests, execute the following commands from within the source directory:

`c:\Projects\rb> set qt_dir=c:\Qt\2009.03` 

`c:\Projects\rb> rake`

After the build you should find the package at __.\\bin\\Win32\\Package\\RedBullPlayer.exe__

__Note:__ _Checkout the sources to a directory *near* the drives root directory. This is required as gcc (or any other component) builds up very long combined relative pathnames of included files and this leads to "file not found errors". So use something like c:\Projects\rb or similar to avoid such issues._


### Linux

To compile, package and run the tests, execute the following commands from within the source directory:

`/home/devel/RedBull/> make -f Makefile.unix` 

After the build you should find the package at  __./bin/Linux/Package/RedBullPlayer__


### Mac OS X

To compile, package and run the tests, execute the following commands from within the source directory:

`/home/devel/RedBull/> make -f Makefile.macx` 

After the build you should find the app bundle at  __./bin/MacOSX/Package/RedBullPlayer.app__ 