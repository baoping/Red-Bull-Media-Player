include("globals.pri")
TEMPLATE = subdirs
CONFIG -= debug_and_release
CONFIG += release
SUBDIRS += 	RedBullPlayer \
		Setup


OTHER_FILES += copy_files.bat \
                agentbuild.bat \
                agentbuild_updater.bat \
                Makefile.unix \
                Makefile.macx \
                Makefile.vlc_unix \
                autorun.inf \
                version.nfo \
                unittest.pri \
                globals.pri \
                RakeFile.rb \
                Makefile.macx_ppc \
                README.txt

unix:!x-win32:build_package.commands = \
	LD_LIBRARY_PATH=./ export LD_LIBRARY_PATH && cp -fv $$DESTDIR/libPackage.* $$PWD/bin/Linux/Setup/ && bash $$PWD/Setup/BuildPackage/mkpkg.sh $$PWD/Setup/InstallerStubs/Linux/RedBullPlayer $$PWD/bin/Linux/Setup/QCompress $$PWD/Setup/runtime_dependencies/Linux/ $$DESTDIR $$PWD/bin/Linux/Package RedBullPlayer

x-win32:build_package.commands = \
	bash $$PWD/Setup/BuildPackage/mkpkg.sh $$PWD/Setup/InstallerStubs/Win32/RedBullPlayer.exe $$PWD/bin/Linux/Setup/QCompress $$PWD/Setup/runtime_dependencies/Win32/ $$DESTDIR $$PWD/bin/x-Win32/Package RedBullPlayer.exe

macx:build_package.commands = \
	bash $$PWD/Setup/BuildPackage/mkappbundle.sh $$CURR_CONFIG

contains( CONFIG, ppc ) {
macx:build_package.commands = \
        bash $$PWD/Setup/BuildPackage/mkappbundle.sh $$CURR_CONFIG ppc
}

win32:batchfile = $$PWD/Setup/BuildPackage/mkpkg.bat
win32:installerstub = $$PWD/Setup/InstallerStubs/Win32/RedBullPlayer.exe
win32:qcompress = $$PWD/bin/Win32/Setup/QCompress.exe
win32:runtime_deps = $$PWD/Setup/runtime_dependencies/Win32//
win32:artefacts_dir = $$DESTDIR//
win32:package_dir = $$PWD/bin/Win32/Package//
win32:package_name = RedBullPlayer.exe
win32:pkg_build_dir = $$PWD/pkg_build_dir//
win32:rootDir = $$PWD//
win32:build_package.commands = $$replace(batchfile, /,\) \
	$$replace(installerstub, /,\) \
	$$replace(qcompress, /,\) \
	$$replace(runtime_deps, /,\) \
	$$replace(artefacts_dir, /,\) \
	$$replace(package_dir, /,\) \
	$$replace(package_name, /,\) \
	$$replace(pkg_build_dir, /,\) \
	$$replace(rootDir, /,\)

win32:runtime_deps_test = $$PWD/Setup/runtime_dependencies/Win32Test/
win32:runtime_deps = $$PWD/Setup/runtime_dependencies/Win32/
win32:artefacts_dir = $$DESTDIR/

win32:setup_test_dir.commands = \
    xcopy /Y /S /I $$replace(artefacts_dir, /,\)* $$replace(artefacts_dir, /,\)\..\UnitTests\ & \
    xcopy /Y /S /I $$replace(runtime_deps_test, /,\)* $$replace(artefacts_dir, /,\)\..\UnitTests\ & \
    xcopy /Y /S /I $$replace(runtime_deps, /,\)* $$replace(artefacts_dir, /,\)\..\UnitTests\


unix:!macx:setup_test_dir.commands = cp -Rvuf $$DESTDIR/* $$PWD/Setup/runtime_dependencies/Linux/* $$PWD/Setup/runtime_dependencies/LinuxTest/* $$DESTDIR/../UnitTests
macx:setup_test_dir.commands = cp -Rvf $$DESTDIR/*.dylib $$DESTDIR/../UnitTests && ln -s $$PWD/3rdParty/MacOSX/Frameworks  $$DESTDIR/../Frameworks

QMAKE_EXTRA_TARGETS += build_package \
                        setup_test_dir
