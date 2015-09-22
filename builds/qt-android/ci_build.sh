#!/usr/bin/env bash
################################################################################
#  THIS FILE IS 100% GENERATED BY ZPROJECT; DO NOT EDIT EXCEPT EXPERIMENTALLY  #
#  Please refer to the README for information about making permanent changes.  #
################################################################################

NDK_VER=android-ndk-r10e
NDK_ABI_VER=4.9

if [ $TRAVIS_OS_NAME == "linux" ]
then
    NDK_PLATFORM=linux-x86_64
elif [ $TRAVIS_OS_NAME == "osx" ]
then
    NDK_PLATFORM=darwin-x86_64
else
    echo "Unsupported platform $TRAVIS_OS_NAME"
    exit 1
fi

export FILENAME=$NDK_VER-$NDK_PLATFORM.bin

(cd '/tmp' \
    && wget http://dl.google.com/android/ndk/$FILENAME \
    && chmod a+x $FILENAME \
    && ./$FILENAME &> /dev/null ) || exit 1
unset FILENAME

export ANDROID_NDK_ROOT="/tmp/$NDK_VER"
export TOOLCHAIN_PATH="$ANDROID_NDK_ROOT/toolchains/arm-linux-androideabi-$NDK_ABI_VER/prebuilt/$NDK_PLATFORM/bin"
export TOOLCHAIN_NAME="arm-linux-androideabi-$NDK_ABI_VER"
export TOOLCHAIN_HOST="arm-linux-androideabi"
export TOOLCHAIN_ARCH="arm"

export ZMQ_ROOT="/tmp/zmq"
git clone https://github.com/zeromq/libzmq $ZMQ_ROOT

export CZMQ_ROOT="/tmp/czmq"
git clone https://github.com/zeromq/czmq $CZMQ_ROOT

export MICROHTTPD_ROOT="/tmp/microhttpd"
git clone http://ftp.gnu.org/gnu/libmicrohttpd/ $MICROHTTPD_ROOT

export CURL_ROOT="/tmp/curl"
git clone https://github.com/bagder/curl $CURL_ROOT

source ./build.sh

################################################################################
#  THIS FILE IS 100% GENERATED BY ZPROJECT; DO NOT EDIT EXCEPT EXPERIMENTALLY  #
#  Please refer to the README for information about making permanent changes.  #
################################################################################
