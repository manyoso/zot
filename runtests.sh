#!/bin/sh

cd `dirname $0`

export BASENAME=${PWD##*/}
export SCRIPTDIR=$PWD
export BUILDDIR=$PWD/build

/bin/sh $SCRIPTDIR/build.sh debug

echo "\nRunning unit tests...\n"

$BUILDDIR/bin/"zottests" "$@"
