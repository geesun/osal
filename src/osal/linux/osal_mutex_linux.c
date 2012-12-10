#include "osal_mutex_os.h"

static pthread_mutex_t  g_osal_table_mutex;

void osal_mutex_self_mutex_init()
{
    pthread_mutex_init((pthread_mutex_t *)&g_osal_table_mutex, NULL);
}

void osal_mutex_self_mutex_exit()
{
    pthread_mutex_destroy(&g_osal_table_mutex);
}

void osal_mutex_self_mutex_lock()
{
     pthread_mutex_lock(&g_osal_table_mutex);
}

void osal_mutex_self_mutex_unlock()
{
     pthread_mutex_unlock(&g_osal_table_mutex);
}


void osal_os_mutex_init(osal_mutex_id_t * id)
{
    pthread_mutexattr_t mutex_attr ;

    pthread_mutexattr_init(&mutex_attr);
    
    pthread_mutex_init(id, &mutex_attr);

}
