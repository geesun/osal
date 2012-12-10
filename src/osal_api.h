#ifndef __OSAL_API_H__
#define __OSAL_API_H__

/****************** mutex ***********************************/
int osal_mutex_sys_init(unsigned int max_mutex_num);
void osal_mutex_sys_exit();
int osal_mutex_init(unsigned int * mutex_id,const char * func, unsigned int line);
int osal_mutex_destroy(unsigned int mutex_id);
int osal_mutex_lock(unsigned int mutex_id,const char * taken_func, int taken_line);
int osal_mutex_unlock(unsigned int mutex_id);
int osal_mutex_trylock(unsigned int mutex_id);

void osal_mutex_info_show();

#define cs_mutex_init(id, name,opt)     osal_mutex_init(id,__FUNCTION__,__LINE__)  
#define cs_mutex_destroy(id)            osal_mutex_destroy(id)  
#define cs_mutex_lock(id)               osal_mutex_lock(id,__FUNCTION__,__LINE__)
#define cs_mutex_unlock(id)             osal_mutex_unlock(id)
#define cs_mutex_trylock(id)            osal_mutex_trylock(id)


#endif
