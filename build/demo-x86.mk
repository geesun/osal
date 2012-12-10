include $(TOOL_ROOT)/configs/x86-linux.mk
include $(TOOL_ROOT)/cmn.mk



TARGET_STATIC_LIB=libosal-x86.a
TARGET_STATIC_SRCS= \
				   $(CMN_SRCS)  \
				   $(wildcard src/osal/linux/*.c)  \
				   $(wildcard src/util/linux/*.c)  \


TARGET_STATIC_CFLAGS=$(CMN_CFLAGS)  -Isrc/osal/linux -Isrc/util/linux


TARGET_SHARE_LIB=
TARGET_SHARE_SRCS= \

TARGET_SHARE_CFLAGS= -fPIC
TARGET_SHARE_LD_FLAGS= 

TARGET_PROG =cs-osal-test
TARGET_PROG_SRCS = \
				   $(CMN_SRCS)  \
				   $(wildcard src/osal/linux/*.c)  \
				   $(wildcard src/util/linux/*.c)  \
				   $(wildcard test/*.c) 
			
TARGET_PROG_CFLAGS = $(CMN_CFLAGS)  -Isrc/osal/linux -Isrc/util/linux
TARGET_PROG_LD_FLAGS =$(CMN_LDFLAGS) 

#TARGET_KERNEL=
#TARGET_KERNEL_SRCS=
#
#TARGET_KERNEL_INC=
#TARGET_KERNEL_CFLAGS= 
#
#
RELEASE_SOURCES=$(CMN_REL_SRCS) \

