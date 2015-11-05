#!/bin/sh

cd `dirname $0`

export BASENAME=${PWD##*/}
export SCRIPTDIR=$PWD
export BUILDDIR=$PWD/build

gdb --args $BUILDDIR/bin/$BASENAME "$@"
