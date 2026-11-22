#!/bin/sh
# configure.sh: openmw-rtx for x86_64 macOS (AMDRadeonRDNA4's rig) into out/mac/build: Homebrew's Clang 23 against the SDK's libc++
# (nothing of Homebrew at run time); macOS 26 for the SDK's libc++ floating-point from_chars, the vcpkg set in out/mac/installed, no Qt programs, no tests.
set -eu
cd "$(dirname "$0")/../.."
SDK=$(xcrun --show-sdk-path)
LLVM=/opt/homebrew/opt/llvm/bin
# Nothing of Homebrew's found (its Mesa libGL, its arm64 libusb, its Lua headers): pkg-config reads the set alone.
export PKG_CONFIG_LIBDIR=$PWD/out/mac/installed/x64-osx-openmw/lib/pkgconfig
cmake -S . -B out/mac/build -G Ninja \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DCMAKE_C_COMPILER=$LLVM/clang -DCMAKE_CXX_COMPILER=$LLVM/clang++ \
  -DCMAKE_OSX_ARCHITECTURES=x86_64 -DCMAKE_OSX_DEPLOYMENT_TARGET=26.0 -DCMAKE_IGNORE_PREFIX_PATH="/opt/homebrew;/usr/local;/opt/X11;/usr/X11R6;/usr/X11" -DCMAKE_OSX_SYSROOT="$SDK" \
  -DCMAKE_CXX_FLAGS="-nostdinc++ -isystem $SDK/usr/include/c++/v1" \
  -DCMAKE_TOOLCHAIN_FILE=$PWD/out/mac/vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DVCPKG_TARGET_TRIPLET=x64-osx-openmw -DVCPKG_INSTALLED_DIR=$PWD/out/mac/installed -DVCPKG_MANIFEST_MODE=OFF \
  -DOPENMW_QT=OFF -DBUILD_TESTING=OFF "$@"
