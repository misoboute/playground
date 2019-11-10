if [ ! -d ~/build/playground ] ; then
    mkdir ~/build/playground
fi
pushd ~/build/playground
cmake -G Xcode \
    -DCMAKE_PREFIX_PATH=/usr/local/swissmeda/Qt5.9.1 \
    -DCMAKE_OSX_SYSROOT:PATH="/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk" \
    ../../src/playground
popd
