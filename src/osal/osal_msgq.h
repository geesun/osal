#ifndef __OSAL_MSGQ_H__
#define __OSAL_MSGQ_H__
#include "osal_mutex.h"

typedef struct{
    unsigned int    msgq_id; 
    
}osal_msgq_info_t;

typedef struct{
    osal_thread_id_t     thread_id;
    osal_mutex_t         mutex_id;
}osal_msgq_task_info_t;





#endif

