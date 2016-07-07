#!/bin/sh
export PATH=$PATH:/home/user/ti-processor-sdk-linux-am335x-evm-02.00.02.11/linux-devkit/sysroots/x86_64-arago-linux/usr/bin
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
