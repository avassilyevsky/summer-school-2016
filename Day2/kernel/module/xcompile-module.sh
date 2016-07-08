#!/bin/sh
TARGET="192.168.99.18"
export PATH=$PATH:/home/user/ti-processor-sdk-linux-am335x-evm-02.00.02.11/linux-devkit/sysroots/x86_64-arago-linux/usr/bin
export MOD_NAME=queue-drv
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- && scp $MOD_NAME.ko root@$TARGET: