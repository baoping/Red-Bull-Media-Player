macx:generate_library_list.commands = echo $$TARGET >> $$DESTDIR/../Setup/DYNLIBS
macx:POST_TARGETDEPS += generate_library_list
macx:QMAKE_EXTRA_TARGETS += generate_library_list

#macx:copy_to_bundle.commands = cp $$DESTDIR/lib$${TARGET}.dylib $$DESTDIR/Player.app/Contents/MacOS/
#macx:POST_TARGETDEPS += copy_to_bundle
#macx:QMAKE_EXTRA_TARGETS += copy_to_bundle
macx:QMAKE_POST_LINK = cp $$DESTDIR/lib$${TARGET}.dylib $$DESTDIR/Player.app/Contents/MacOS/
