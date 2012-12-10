#include "osal_mutex_os.h"

static cyg_mutex_t g_osal_table_mutex;

void osal_mutex_self_mutex_init()
{
    cyg_mutex_init(&osal_mut_table_mutex);
}

void osal_mutex_self_mutex_exit()
{
    cyg_mutex_destroy(&osal_mut_table_mutex);
}

void osal_mutex_self_mutex_lock()
{
    cyg_mutex_lock(&g_osal_table_mutex);
}

void osal_mutex_self_mutex_unlock()
{
    cyg_mutex_unlock(&g_osal_table_mutex);
}

