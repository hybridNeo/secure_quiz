#!/bin/bash

CURDIR=`pwd`

# This expects that this is place as a first level folder relative to the other
# OP-TEE folder in a setup using default repo configuration as described by the
# documentation in optee_os (README.md)
ROOT=${PWD}
ROOT=`dirname $ROOT`
# Path to the toolchain
export PATH=${ROOT}x/toolchains/aarch32/bin:$PATH

# Path to the TA-DEV-KIT coming from optee_os
export TA_DEV_KIT_DIR=${ROOT}/optee_os/out/arm-plat-hikey/export-ta_arm64

# Path to the client library (GP Client API)
export TEEC_EXPORT=${ROOT}/optee_client/out/export

export PLATFORM=hisi
export PLATFORM_FLAVOR=hikey

# Toolchain prefix for user space code (normal world)
HOST_CROSS_COMPILE=/home/rahulmahadev/hikey_optee/toolchains/gcc-linaro-aarch64-linux-gnu-4.9-2014.09_linux/bin/aarch64-linux-gnu-
# Build the host application

#make CROSS_COMPILE=$HOST_CROSS_COMPILE $@
cd $CURDIR/../
make build-neo
# Toolchain prefix for the Trusted Applications
TA_CROSS_COMPILE=/home/rahulmahadev/hikey_optee/toolchains/gcc-linaro-aarch64-linux-gnu-4.9-2014.09_linux/bin/aarch64-linux-gnu-
# Build the Trusted Application
cd $CURDIR/ta
make O=$CURDIR/ta/out CROSS_COMPILE=$TA_CROSS_COMPILE $@
