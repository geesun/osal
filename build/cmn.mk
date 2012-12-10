CMN_SRCS=   \
		$(wildcard src/osal/*.c) \
		$(wildcard src/util/*.c) \

ifeq ($(PRODUCT),)
	PRODUCT=product
endif

CMN_REL_SRCS=   \
				src/osal_api.h:inc/osal_api.h\
				src/util_api.h:inc/util_api.h\

KMOD_RES_SRCS=  \


CMN_CFLAGS=-Isrc -g -Wall -Isrc/  -Isrc/osal/ -Isrc/util/
CMN_LDFLAGS=-Lcli/libs/ -lpthread -lrt  
