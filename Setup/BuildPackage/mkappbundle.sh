#!/bin/bash

function createBundle() {
    cleanPackageDir
    copyToBundleDir
    macDeployQt
    copyToPackageDir

}

function cleanPackageDir() {
    echo "Clean Package dir..."
    rm -rfv $CURRDIR/bin/MacOSX/Package/

}

function copyToBundleDir() {
   echo "Copy to bundle Dir..."

   rm -rfv $BUNDLEDIR/Contents/Frameworks/*dylib

   cp $DESTDIR/plugins.xml $BUNDLEDIR/Contents/MacOS
   cp $DESTDIR/config.ini $BUNDLEDIR/Contents/MacOS
   cp $DESTDIR/log4qt.properties $BUNDLEDIR/Contents/MacOS
   cp $DESTDIR/version.nfo $BUNDLEDIR/Contents/MacOS
   cp $DESTDIR/styles_Macx.xml $BUNDLEDIR/Contents/MacOS
   cp -Rv $DESTDIR/media $BUNDLEDIR/Contents/MacOS
   cp $DESTDIR/Updtr $BUNDLEDIR/Contents/MacOS

echo "ARCH: $ARCH"
if [ "$ARCH" == "ppc" ]; then
    rm -rf $BUNDLEDIR/Contents/Frameworks/VLCKit.framework
    cp -Rv $CURRDIR/3rdParty/MacOSX/ppc/VLCKit.framework $BUNDLEDIR/Contents/Frameworks/
else
    cp -Rv $CURRDIR/3rdParty/MacOSX/Frameworks $BUNDLEDIR/Contents/
fi
}

function macDeployQt() {
    echo "Call macdeployqt..."

    cd $DESTDIR
    arg=""
    for line in $DYLIBS; do
        if [ -e "$BUNDLEDIR/Contents/MacOS/$line" ]; then
            arg=$arg,$line
        fi
        if [ -e "$BUNDLEDIR/Contents/MacOS/lib$line.dylib" ]; then
            arg=$arg,lib$line.dylib
        fi
    done
#    echo $arg
    $MACDEPLOYQT $BUNDLEDIR -extraLibs=$arg

    cd $CURRDIR
}

function copyToPackageDir() {
    echo "Create zip Package..."
    mkdir $CURRDIR/bin/MacOSX/Package

    cd $DESTDIR
    zip -r -y Player.app.zip Player.app
    cp Player.app.zip $CURRDIR/bin/MacOSX/Package/
    cp -Rv Player.app $CURRDIR/bin/MacOSX/Package/
    mv $CURRDIR/bin/MacOSX/Package/Player.app $CURRDIR/bin/MacOSX/Package/RedBullPlayer.app
}

# exec starts here
CURRDIR=`pwd`
DESTDIR=$CURRDIR/bin/MacOSX/$1
BUNDLEDIR=$DESTDIR/Player.app
MACDEPLOYQT=$CURRDIR/bin/MacOSX/Setup/macdeployqt
DYLIBS=`cat $CURRDIR/bin/MacOSX/Setup/DYNLIBS`
ARCH="$2"


echo "Make Appbundle..."
echo $ARCH



# exit 0

createBundle

echo "DESTDIR: $DESTDIR"
# echo "CURRDIR: $CURRDIR"
# echo "DYLIBS: $DYLIBS"



