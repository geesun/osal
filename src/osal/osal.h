#ifndef __OSAL_H__
#define __OSAL_H__

typedef enum{
    OSAL_OK = 0,
    OSAL_E_FORMAT,
    OSAL_E_NO_MEMORY,
    OSAL_E_PARAM,
    OSAL_E_NOT_FOUND,
    OSAL_E_ERROR = -1,
}osal_status_t;


#define OSAL_LOG(arg,...) \
    printf("%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define OSAL_LOG_INFO(arg,...) \
    printf("%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define OSAL_LOG_MIN(arg,...) \
    printf("%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define OSAL_LOG_MAJ(arg,...) \
    printf("%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define OSAL_LOG_CRI(arg,...) \
    printf("%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define OSAL_DEBUG(arg,...) \
    printf("%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define OSAL_MALLOC(size)        malloc((size))
#define OSAL_FREE(ptr)           if((ptr) != NULL){ free((ptr)); (ptr) = NULL; }

#endif
