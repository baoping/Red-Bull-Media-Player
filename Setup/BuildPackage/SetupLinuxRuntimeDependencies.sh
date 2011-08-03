#!/bin/bash

function collectLibs() {
	echo "collectLibs..."

	# cd $SEARCHROOT 
	find $SEARCHROOT -perm /u+x -type f -exec ldd {} \; > $HOME/dependency_libs
	# cat $HOME/dependency_libs
	DEPENDENY_LIBS=`cat $HOME/dependency_libs | cut -d' ' -f1,3 | sort | uniq | grep /usr/lib/ | cut -d' ' -f2`
	echo $DEPENDENY_LIBS
}

function copyToTargetFolder() {
	echo "copyToTargetFolder..."
	cd $TARGETFOLDER
	echo $DEPENDENY_LIBS
	for currlib in $DEPENDENY_LIBS; do
		LIBNAME=`basename $currlib `
		if [[ "$LIBNAME" =~ ^libX.* ]]; then
			echo "skip X11 library: $LIBNAME"
		else
			echo "Copy library: $currlib..."
			cp -Lfv $currlib  .
		fi
	done
}

function cleanup() {
	rm -f $HOME/dependency_libs
}

PWD_CALC=`pwd`
PWD="$PWD_CALC"
SEARCHROOT=$1
TARGETFOLDER=$2

if [ "$SEARCHROOT" == "" ]; then
	echo "No SEARCHROOT given!";
	exit 1
fi
if [ "$TARGETFOLDER" == "" ]; then
	echo "No TARGETFOLDER given!";
	exit 1
fi

echo "PWD: $PWD"
echo "SEARCHROOT: $SEARCHROOT"
echo "TARGETFOLDER: $TARGETFOLDER"

collectLibs
copyToTargetFolder
cleanup
