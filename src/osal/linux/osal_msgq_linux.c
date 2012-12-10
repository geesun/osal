#include <sys/shm.h>
#include <stdio.h>
#include "osal.h"
#include "osal_msgq_os.h"

static unsigned char * g_osal_msgq_linux_buf = NULL;

osal_status_t osal_msqq_mem_init(int size)
{
    char *fname = "/var/run/messageQ";
    key_t key = ftok(fname,2);

    int shmid = shmget(key,size,IPC_CREAT|0600);

    if(shmid == -1){
        OSAL_LOG("share memory get failed \n");
        return OSAL_E_ERROR;
    }

    g_osal_msgq_linux_buf = (unsigned char *) shmat(shmid,NULL,0);

    if(g_osal_msgq_linux_buf == NULL){
        OSAL_LOG("share memory attach failed \n");
        return OSAL_E_ERROR;
    }
    
    return OSAL_OK;
}


void osal_msqq_mem_exit()
{
    shmdt(g_osal_msgq_linux_buf);
    g_osal_msgq_linux_buf = NULL;

    /*we don't delete the memory from the system*/
}
