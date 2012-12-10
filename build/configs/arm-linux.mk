CROSS_COMPILE=/auto/project2/sw/toolchains/le-openwrt-gcc-4.3.3+cs_uClibc-0.9.32-rc3_eabi-pthread/usr/bin/arm-openwrt-linux-
CC=$(CROSS_COMPILE)gcc
AS=$(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AR = $(CROSS_COMPILE)ar
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdum



CFLAGS=

CFLAGS+=

LD_PATH=
LD_FLAGS=


OS=linux
ARCH=arm

KERNEL_BUILD := /home/geesun/src/lynxe_latest/openwrt-10.03/build_dir/linux-lynxe/linux-2.6.32.3/
LYNXE_ROOT=/home/geesun/src/lynxe_latest/
