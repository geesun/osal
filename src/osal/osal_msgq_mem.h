#ifndef __OSAL_MSGQ_MEM_H__
#define __OSAL_MSGQ_MEM_H__

typedef enum{
    OSAL_MSGQ_MEM_FREE,
    OSAL_MSGQ_MEM_USED,
    OSAL_MSGQ_MEM_HOLD,
    OSAL_MSGQ_MEM_LEAK,
}osal_msgq_mem_state_t;


typedef struct{
    osal_msgq_mem_state_t state;
}osal_msgq_mem_hdr_t;

#endif
