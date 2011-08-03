#!/bin/bash

function help() {
	echo "Usage: buildpkg.sh BUILD_DIR INSTALLER_STUB QCOMPRESS_BIN RUNTIME_DEPS_DIR ARTEFACTS_DIR TARGET_DIR TARGET_NAME"
}

function build() {
	checkDependencies
	setupOrCleanBuildDir
	copyRuntimeDependencies
	setupRuntimeDependenmcies
	copyArtefacts
	fixLibraryNames
	copyInstallerStub
	mkPkg
	copyTargetToTargetDir
	deletePkgBuildDir
}

function setupOrCleanBuildDir() {
	if [ -e $PKG_BUILD_DIR ]; then
		echo -n "Delete old pkg build dir: $PKG_BUILD_DIR..."
		rm -rf $PKG_BUILD_DIR
		echo "Done"
		
	fi
	echo -n "Create pkg build dir..."
	mkdir $PKG_BUILD_DIR
	mkdir $PKG_BUILD_DIR/bin
	echo "Done"
	
	if [ ! -e $TARGET_DIR ]; then
		echo -n "Create Target dir $TARGET_DIR..."
		mkdir $TARGET_DIR
		echo "Done"
	fi
	
	if [ -e $TARGET_DIR/$TARGET_NAME ]; then
		echo "Delete old package $TARGET_DIR/$TARGET_NAME"
		rm -f $TARGET_DIR/$TARGET_NAME
	fi
}

function deletePkgBuildDir() {
	echo -n "Deleting Package Build Dir..."
	rm -rf $PKG_BUILD_DIR
	echo "Done"
}

function copyInstallerStub() {
	echo -n "Copy Installer stub..."
	cp $INSTALLER_STUB $PKG_BUILD_DIR/$TARGET_NAME
	echo "Done"

}

function checkDependencies() {
	if [ ! -e $BUILD_DIR ]; then
		echo "BUILD_DIR: $BUILD_DIR does not exist!"
		exit 1
	fi

	if [ ! -e $INSTALLER_STUB ]; then
		echo "INSTALLER_STUB: $INSTALLER_STUB does not exist!"
		exit 1
	fi

	if [ ! -e $QCOMPRESS_BIN ]; then
		echo "QCOMPRESS_BIN: $QCOMPRESS_BIN does not exist!"
		exit 1
	fi
	
	if [ ! -e $RUNTIME_DEPS_DIR ]; then
		echo "RUNTIME_DEPS_DIR: $RUNTIME_DEPS_DIR does not exist!"
		exit 1
	fi

	if [ ! -e $ARTEFACTS_DIR ]; then
		echo "ARTEFACTS_DIR: $ARTEFACTS_DIR does not exist!"
		exit 1
	fi
}

function setupRuntimeDependenmcies() {
	bash Setup/BuildPackage/SetupLinuxRuntimeDependencies.sh $ARTEFACTS_DIR/ $FULL_PKG_BUILD_DIR/bin

}

function copyRuntimeDependencies() {
	echo -n "Copy runtime dependencies to build dir..."
	cp -r $RUNTIME_DEPS_DIR/* $PKG_BUILD_DIR/bin
	echo "Done"
}

function copyArtefacts() {
	echo -n "Copy build artefacts to build dir..."
	cp -r $ARTEFACTS_DIR/* $PKG_BUILD_DIR/bin
	echo "Done"
}

function fixLibraryNames() {
	#if [ `ls -la *.so.1.0.0  &> /dev/null` ]; then
		OLDDIR=`pwd`
		cd  $PKG_BUILD_DIR/bin
		for i in `ls *.so.1.0.0`; do
			newName=`echo $i | cut -d'.' -f1`
			newName="$newName.so.1"
			mv $i $newName
		done
		cd $OLDDIR
	#fi
}

function mkPkg() {
	OLDDIR=`pwd`
	echo "cd to $PKG_BUILD_DIR/bin"
	cd $PKG_BUILD_DIR/bin
	echo "Build Package"
	$QCOMPRESS_BIN directory . . ../$TARGET_NAME
	cd $OLDDIR
}

function copyTargetToTargetDir() {
	echo "Move $PKG_BUILD_DIR/$TARGET_NAME to $TARGET_DIR/$TARGET_NAME"
	mv $PKG_BUILD_DIR/$TARGET_NAME $TARGET_DIR/$TARGET_NAME
	chmod +x $TARGET_DIR/$TARGET_NAME
}

# exec starts here
PKG_BUILD_DIR="./pkg_build_dir/"
CURRDIR=`pwd`
FULL_PKG_BUILD_DIR="$CURRDIR/pkg_build_dir/"
INSTALLER_STUB=$1
QCOMPRESS_BIN=$2
RUNTIME_DEPS_DIR=$3
ARTEFACTS_DIR=$4
TARGET_DIR=$5
TARGET_NAME=$6


if [ "$INSTALLER_STUB" == "" ]; then
	help;
	exit 1
else
	echo "INSTALLER_STUB: $INSTALLER_STUB"
fi
if [ "$QCOMPRESS_BIN" == "" ]; then
	help;
	exit 1
else
	echo "QCOMPRESS_BIN: $QCOMPRESS_BIN"
fi
if [ "$RUNTIME_DEPS_DIR" == "" ]; then
	help;
	exit 1
else
	echo "RUNTIME_DEPS_DIR: $RUNTIME_DEPS_DIR"
fi
if [ "$ARTEFACTS_DIR" == "" ]; then
	help;
	exit 1
else
	echo "ARTEFACTS_DIR: $ARTEFACTS_DIR"
fi
if [ "$TARGET_DIR" == "" ]; then
	help;
	exit 1
else
	echo "TARGET_DIR: $TARGET_DIR"
fi
if [ "$TARGET_NAME" == "" ]; then
	help;
	exit 1
else
	echo "TARGET_NAME: $TARGET_NAME"
fi

build
