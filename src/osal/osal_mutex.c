#include "osal_mutex.h"

static osal_mutex_t *  g_osal_mutexs = NULL; 
static unsigned int    g_osal_mutex_num = 0; 


int osal_mutex_sys_init(unsigned int max_mutex_num)
{
    if(g_osal_mutex_num != 0){
        OSAL_MUTEX_LOG("Mutex system already init \n");
        return -1;
    }

    g_osal_mutexs = OSAL_MUTEX_MALLOC(sizeof(osal_mutex_t) * max_mutex_num);

    if(g_osal_mutexs == NULL){
        OSAL_MUTEX_LOG("Mutex init failed, No memory \n");
        return -1;
    }

    memset(g_osal_mutexs, 0x00,sizeof(osal_mutex_t) * max_mutex_num);

    g_osal_mutex_num = max_mutex_num;
    
    osal_mutex_self_mutex_init();
    return 0;
}


void osal_mutex_sys_exit()
{
    OSAL_MUTEX_FREE(g_osal_mutexs);
    g_osal_mutex_num = 0;
    osal_mutex_self_mutex_exit();
}


int osal_mutex_init(unsigned int * mutex_id,const char * func, unsigned int line)
{
    unsigned int id ;
    int ret = 0;

    /*Not init system*/
    if(g_osal_mutex_num == 0){
        osal_mutex_sys_init(OSAL_MUTEX_DFLT_NUM);
    }

    if(mutex_id == NULL){
        OSAL_MUTEX_LOG("Invalid param \n");
        return -1;
    }

    osal_mutex_self_mutex_lock();
    for(id = 0 ; id < g_osal_mutex_num; id++){
        if(!g_osal_mutexs[id].valid){
            break;
        }
    }
    
    if(id == g_osal_mutex_num){
        OSAL_MUTEX_LOG("No Free Mutex handler \n");
        ret = -1;
        goto Exit;
    }
    
    memset(&g_osal_mutexs[id],0x00,sizeof(osal_mutex_t));
    
#ifdef OSAL_DEBUG
    if(func != NULL){
        snprintf(g_osal_mutexs[id].creater,OSAL_MUTEX_INFO_LEN,"%s:%d",func, line);
    }
#endif
    *mutex_id = id;
    g_osal_mutexs[id].valid = 1;
    g_osal_mutexs[id].taken = 0;
    osal_os_mutex_init(&g_osal_mutexs[id].id);

Exit:
    osal_mutex_self_mutex_unlock();
    return ret;
}

int osal_mutex_destroy(unsigned int mutex_id)
{
    int ret = 0;

    osal_mutex_self_mutex_lock();
    if(mutex_id >= g_osal_mutex_num 
            || !g_osal_mutexs[mutex_id].valid){
        OSAL_MUTEX_LOG("Invalid mutex id %d \n",mutex_id);
        ret =  -1;
        goto Exit;
    }
    
    osal_os_mutex_destory(&g_osal_mutexs[mutex_id].id);
    g_osal_mutexs[mutex_id].valid = 0;

Exit:
    osal_mutex_self_mutex_unlock();
    return ret;
}

int osal_mutex_lock(unsigned int mutex_id,const char * taken_func, int taken_line)
{
    int ret = 0;

    if(mutex_id >= g_osal_mutex_num 
            || !g_osal_mutexs[mutex_id].valid){
        OSAL_MUTEX_LOG("Invalid mutex id %d \n",mutex_id);
        ret =  -1;
        goto Exit;
    }

#ifdef OSAL_DEBUG
    memset(g_osal_mutexs[mutex_id].wait, 0x00,OSAL_MUTEX_INFO_LEN);
    
    if(taken_func != NULL){
        snprintf(g_osal_mutexs[mutex_id].wait,OSAL_MUTEX_INFO_LEN,"%s:%d",taken_func, taken_line);
    }
#endif
    osal_os_mutex_lock(&g_osal_mutexs[mutex_id].id);

#ifdef OSAL_DEBUG
    memset(g_osal_mutexs[mutex_id].wait, 0x00,OSAL_MUTEX_INFO_LEN);
    memset(g_osal_mutexs[mutex_id].owner, 0x00,OSAL_MUTEX_INFO_LEN);
        
    if(taken_func != NULL){
        snprintf(g_osal_mutexs[mutex_id].owner,OSAL_MUTEX_INFO_LEN,"%s:%d",taken_func, taken_line);
    }
#endif
    g_osal_mutexs[mutex_id].taken = 1;

Exit:
    return ret;
}

int osal_mutex_unlock(unsigned int mutex_id)
{
    int ret = 0;

    if(mutex_id >= g_osal_mutex_num 
            || !g_osal_mutexs[mutex_id].valid){
        OSAL_MUTEX_LOG("Invalid mutex id %d \n",mutex_id);
        ret =  -1;
        goto Exit;
    }
    if(!g_osal_mutexs[mutex_id].taken){
        OSAL_MUTEX_LOG("mutex was not taken,id %d \n",mutex_id);
        ret =  -1;
        goto Exit;

    }
    osal_os_mutex_unlock(&g_osal_mutexs[mutex_id].id);

#ifdef OSAL_DEBUG
    memset(g_osal_mutexs[mutex_id].owner, 0x00,OSAL_MUTEX_INFO_LEN);
#endif
    g_osal_mutexs[mutex_id].taken = 0;
Exit:
    return ret;
}



int osal_mutex_trylock(unsigned int mutex_id)
{
    int ret = 0;

    if(mutex_id >= g_osal_mutex_num 
            || !g_osal_mutexs[mutex_id].valid){
        OSAL_MUTEX_LOG("Invalid mutex id %d \n",mutex_id);
        ret =  -1;
        goto Exit;
    }

    osal_os_mutex_trylock(&g_osal_mutexs[mutex_id].id);

Exit:
    return ret;
}

void osal_mutex_info_show()
{
#ifdef OSAL_DEBUG
    int i ; 
    printf("%-4s\t%-5s\t%-32s\t%-32s\t%-32s \n","ID","Taken","Creater","Owner","Wait");
    for(i = 0 ; i < g_osal_mutex_num; i++){
        if(g_osal_mutexs[i].valid){
            printf("%d\t%-5s\t%-32s\t%-32s\t%-32s \n",i,
                    g_osal_mutexs[i].taken?"taken":"free",
                    g_osal_mutexs[i].creater,
                    g_osal_mutexs[i].owner,
                    g_osal_mutexs[i].wait);
        }
    }
#endif
}
