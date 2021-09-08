#!/bin/bash
set -e

ANDROID_LIB=../../jnilibs
ANDROID_INCLUDE=./include

mkdir -p $ANDROID_INCLUDE

 # 64-bit MIPS (mips-64-android,mips64) currently does not build since
 # llvm will not compile for the R6 version of the ISA without Nan2008
 # and the gcc toolchain used by the Android build setup requires those
 # two options together.
 for archs in arm-32-android,armeabi arm-32-android-armv7s,armeabi-v7a arm-64-android,arm64-v8a mips-32-android,mips x86-64-android-sse41,x86_64 x86-32-android,x86 ; do
     IFS=,
     set $archs
     HL_TARGET=$1 ANDROID_ABI=bin/$2 make build.exposure.android
     mkdir -p $ANDROID_LIB/$2
     mkdir -p $ANDROID_INCLUDE/$2
     cp ./bin/$2/exposure.a $ANDROID_LIB/$2/
     cp ./bin/$2/exposure.h ./include/$2/
 done