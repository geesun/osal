#ifndef __OSAL_MUTEX_H__
#define __OSAL_MUTEX_H__
#include "osal_mutex_os.h"

#define OSAL_MUTEX_DFLT_NUM         32

#define OSAL_MUTEX_INFO_LEN        128

#define OSAL_DEBUG 

typedef struct{
    unsigned char       valid;
    osal_mutex_id_t     id;
    unsigned char       taken;
#ifdef OSAL_DEBUG
    char                creater[OSAL_MUTEX_INFO_LEN];
    char                owner[OSAL_MUTEX_INFO_LEN];
    char                wait[OSAL_MUTEX_INFO_LEN];
#endif
}osal_mutex_t;

#define OSAL_MUTEX_LOG(arg,...) \
    printf("%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define OSAL_MUTEX_LOG_INFO(arg,...) \
    printf("%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define OSAL_MUTEX_LOG_MIN(arg,...) \
    printf("%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define OSAL_MUTEX_LOG_MAJ(arg,...) \
    printf("%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define OSAL_MUTEX_LOG_CRI(arg,...) \
    printf("%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define OSAL_MUTEX_DEBUG(arg,...) \
    printf("%s(): "arg, __FUNCTION__,##__VA_ARGS__)




#define OSAL_MUTEX_MALLOC(size)        malloc((size))
#define OSAL_MUTEX_FREE(ptr)           if((ptr) != NULL){ free((ptr)); (ptr) = NULL; }




void osal_mutex_self_mutex_init();
void osal_mutex_self_mutex_exit();

void osal_mutex_self_mutex_lock();
void osal_mutex_self_mutex_unlock();

#endif
