#!/bin/bash


CURRDIR=`pwd`

cd $CURRDIR/3rdParty/MacOSX/Frameworks/VLCKit.framework/
pwd
for lnk in bin Headers lib plugins Resources share; do
    echo "remove $lnk"
    echo "link ./Versions/A/$lnk $lnk"
    rm -fv $lnk
    ln -s ./Versions/A/$lnk $lnk
done

cd ./Versions
rm Current
ln -s A Current

cd ..
rm VLCKit
ln -s Versions/Current/VLCKit VLCKit

cd lib
rm libvlc.dylib
rm libvlccore.dylib
cp libvlc.5.dylib libvlc.dylib
cp libvlccore.4.dylib libvlccore.dylib

cd $CURRDIR/3rdParty/MacOSX/qtanimationframework-2.3/lib/

rm libQtSolutions_AnimationFramework-2.3.dylib
rm libQtSolutions_AnimationFramework-2.3.1.dylib
rm libQtSolutions_AnimationFramework-2.3.1.0.dylib

ln -s libQtSolutions_AnimationFramework-2.3.1.0.0.dylib libQtSolutions_AnimationFramework-2.3.dylib
ln -s libQtSolutions_AnimationFramework-2.3.1.0.0.dylib libQtSolutions_AnimationFramework-2.3.1.dylib
ln -s libQtSolutions_AnimationFramework-2.3.1.0.0.dylib libQtSolutions_AnimationFramework-2.3.1.0.dylib


