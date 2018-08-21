if [ ! -d ~/build/playground ] ; then
    mkdir ~/build/playground
fi
pushd ~/build/playground
cmake -G Xcode \
    -DSM_PLANNING_TOOL_DIR=~/src/planning_program_win \
    -DCMAKE_PREFIX_PATH=/usr/local/swissmeda/Qt5.9.1 \
    -DCMAKE_OSX_SYSROOT:PATH="/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk" \
    ../../src/playground
popd
