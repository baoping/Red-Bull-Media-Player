include( "../../globals.pri" )
x-win32:include("../../x-win32-dll.pri")
TARGET = Widgets
QT += network \
    xml \
    svg \
    sql
TEMPLATE = lib
DEFINES += WIDGETS_LIBRARY
SOURCES += TransparentImage.cpp \
    MirrorWidget.cpp \
    TransformWidget.cpp \
    ClickableLabel.cpp \
    Button.cpp \
    DraggableLabel.cpp \
    Sprite.cpp \
    GridViewTransformWidget.cpp \
    ListViewTransformWidget.cpp \
    CircleProgress.cpp \
    CommandButton.cpp \
    ToogleButton.cpp \
    ImageListHandler.cpp \
    TextBrowser.cpp \
    StatusDisplayButton.cpp \
    RedBullDialog.cpp \
    DiskSpaceButton.cpp \
    AutoResizeImageLabel.cpp \
    UpdateProgressWidget.cpp \
    HelpWidget.cpp \
    StatusCountDisplayButton.cpp \
    InternetAvailabilityButton.cpp
HEADERS += TransparentImage.h \
    MirrorWidget.h \
    TransformWidget.h \
    ClickableLabel.h \
    Button.h \
    DraggableLabel.h \
    Sprite.h \
    GridViewTransformWidget.h \
    ListViewTransformWidget.h \
    CircleProgress.h \
    CommandButton.h \
    ToogleButton.h \
    Enumerations.h \
    ImageListHandler.h \
    TextBrowser.h \
    StatusDisplayButton.h \
    RedBullDialog.h \
    DiskSpaceButton.h \
    AutoResizeImageLabel.h \
    UpdateProgressWidget.h \
    HelpWidget.h \
    StatusCountDisplayButton.h \
    InternetAvailabilityButton.h
win32_libs = $$DESTDIR/Container.dll \
    $$DESTDIR/ToolLib.dll \
    $$DESTDIR/Events.dll \
    $$DESTDIR/Interfaces.dll
unix_libs = $$DESTDIR/libContainer.so \
    $$DESTDIR/libToolLib.so \
    $$DESTDIR/libEvents.so \
    $$DESTDIR/libInterfaces.so
macx_libs = $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib \
    $$DESTDIR/libEvents.1.0.0.dylib \
    $$DESTDIR/libInterfaces.1.0.0.dylib \
    $$DESTDIR/liblog4qt-0.3.1.dylib
include("../../libref.pri")
FORMS += RedBullDialog.ui \
    UpdateProgressWidget.ui
RESOURCES += 
OTHER_FILES += 
