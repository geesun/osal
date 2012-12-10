#ifndef __OSAL_MUTEX_OS_H__
#define __OSAL_MUTEX_OS_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cyg/kernel/kapi.h>

typedef cyg_mutex_t  osal_mutex_id_t;


#define osal_os_mutex_init          cyg_mutex_init 
#define osal_os_mutex_destory       cyg_mutex_destroy

#define osal_os_mutex_lock          cyg_mutex_lock
#define osal_os_mutex_unlock        cyg_mutex_unlock

#define osal_os_mutex_trylock       cyg_mutex_trylock


#endif
