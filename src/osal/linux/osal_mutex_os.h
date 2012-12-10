#ifndef __OSAL_MUTEX_OS_H__
#define __OSAL_MUTEX_OS_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

typedef pthread_mutex_t  osal_mutex_id_t;

void osal_os_mutex_init(osal_mutex_id_t * id);
#define osal_os_mutex_destory       pthread_mutex_destroy

#define osal_os_mutex_lock          pthread_mutex_lock
#define osal_os_mutex_unlock        pthread_mutex_unlock

#define osal_os_mutex_trylock       pthread_mutex_trylock


#endif
