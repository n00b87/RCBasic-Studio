#!/bin/bash

BASEDIR=$(dirname 0)


echo "build journal path: $RCBASIC_BUILD_JOURNAL"

codeblocks --clean --target=Release RCBasic-Studio.cbp
codeblocks --target=Release --build RCBasic-Studio.cbp

if [ ! -e $BASEDIR/bin/Release/RCBasic-Studio ]
then

pushd $RCBASIC_BUILD_MAIN_DIR
echo "export RCBASIC_STUDIO_ERROR=1" >> "$RCBASIC_BUILD_JOURNAL"
popd

else

pushd $RCBASIC_BUILD_MAIN_DIR
echo "export RCBASIC_STUDIO_ERROR=0" >> "$RCBASIC_BUILD_JOURNAL"
popd

fi


