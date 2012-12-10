#include "util_timer_os.h"
#include "util_timer.h"
#include "osal_api.h"

static util_timer_ctrl_t  g_timer_ctrl;
static unsigned char      g_timer_init = 0;

void util_timer_thread();

int util_timer_sys_init(unsigned int max_timer_num)
{
    if(g_timer_init){
        UTIL_TIMER_LOG("Timer system is already init \n");
        return -1;
    }
    
    g_timer_ctrl.act_timer_list = NULL;
    g_timer_ctrl.stop_timer_list = NULL;
    g_timer_ctrl.time_data = UTIL_TIMER_MALLOC(sizeof(util_timer_t) * max_timer_num);

    if(g_timer_ctrl.time_data == NULL){
        UTIL_TIMER_LOG("Timer init failed: No memory \n");
        return -1;
    }

    memset(g_timer_ctrl.time_data, 0x00,sizeof(util_timer_t) * max_timer_num);
    g_timer_ctrl.max_timer_num = max_timer_num;
    
    cs_mutex_init(&g_timer_ctrl.mutex, NULL,0);
    
    util_os_timer_thread_start(util_timer_thread);
    return 0;
}

void util_timer_sys_exit()
{
    if(!g_timer_init){
        return;
    }
    
    if(g_timer_ctrl.time_data != NULL){
        UTIL_TIMER_FREE(g_timer_ctrl.time_data);
    }
    
    g_timer_ctrl.max_timer_num = 0;
    cs_mutex_destroy(g_timer_ctrl.mutex);
    g_timer_init = 0;
}


static util_timer_t *  util_timer_new(
        unsigned int timeout,
        util_timer_handler_t * callback,
        void * data)
{
    int i = 0; 
    util_timer_t * timer = NULL;

    for(i = 0; i <  g_timer_ctrl.max_timer_num; i++){
        timer = &g_timer_ctrl.time_data[i]; 
        if(!timer->valid){
           break; 
        }
    }

    if(i == g_timer_ctrl.max_timer_num){
        UTIL_TIMER_LOG("No Free timer \n");
        return NULL;
    }
    
    timer->valid = 1;
    timer->timeout = timeout;
    timer->offset_time = timeout;
    timer->callback = callback;
    timer->data = data;
    timer->type = TIMER_TYPE_ONCE; /* default type is once timer*/
    timer->state = TIMER_STATE_ACTIVE;
    timer->next = NULL;

    return timer;
}

static void util_timer_free(util_timer_t * timer)
{
    if(timer == NULL){
        return;
    }

    timer->valid = 0;
    timer->next = NULL;
    timer->callback = NULL;
    timer->data = NULL;
    timer->timeout = 0;
}

static int util_timer_insert(util_timer_t * timer)
{
    util_timer_t * item = NULL;
    util_timer_t * prev = NULL;

    if(timer == NULL){
        return -1;
    }
    
    /* insert to header */
    if(g_timer_ctrl.act_timer_list == NULL){
        g_timer_ctrl.act_timer_list = timer;
        return 0;
    }

    for(item = g_timer_ctrl.act_timer_list ; 
            item != NULL; item = item->next){
        /*append it to item previous*/
        if((timer->offset_time - item->offset_time) < 0){
            if(prev != NULL){
                prev->next = timer;
            }
            timer->next = item; 
            /*update next offset*/
            item->offset_time -= timer->offset_time;
            
            /*head*/
            if(prev == NULL){
                g_timer_ctrl.act_timer_list = timer;
            }
            break;
        /*equal to item, set the timer to the next and offset to 0*/
        }else if((timer->offset_time - item->offset_time) == 0){
           timer->next = item->next; 
           item->next = timer;
           timer->offset_time = 0;
           break;
        }else{
            prev = item;
            timer->offset_time -= item->offset_time;
        }
    }
    
    /*last one */
    if(item == NULL){
        prev->next = timer;
    }

    return 0;
}

util_timer_t *  util_timer_add(
        util_timer_type_t type,
        unsigned int timeout,
        util_timer_handler_t * callback,
        void * data)
{
    util_timer_t * timer = NULL;
    unsigned int ret = 0; 

    if(callback == NULL){
        UTIL_TIMER_LOG("Timer callback is NULL \n");
        return UTIL_INVALID_TIMER;
    }

    if(!g_timer_init){
        UTIL_TIMER_LOG("Timer system is not init \n");
        return UTIL_INVALID_TIMER;
    }

    /* someone one maybe call timer add in callback to avoid the deadlock,
     * we no need lock when it runing at myself thread since we already locked it
     */
    if(util_os_thread_id() != g_timer_ctrl.thread_id){
        cs_mutex_lock(g_timer_ctrl.mutex);
    }

    timer = util_timer_new(timeout,callback,data);
    if(timer == NULL){
        UTIL_TIMER_LOG("Can not start a new timer \n");
        ret = UTIL_INVALID_TIMER;
        goto Exit;
    }

    if(type == TIMER_TYPE_CYCLE){
        timer->type = TIMER_TYPE_CYCLE;
    }

    util_timer_insert(timer);

Exit:
    if(util_os_thread_id() != g_timer_ctrl.thread_id){
        cs_mutex_unlock(g_timer_ctrl.mutex);
    }
    return timer;
}

static int util_timer_act_remove(util_timer_t * timer)
{
    util_timer_t * item = NULL;
    util_timer_t * prev = NULL;
    int ret = 0;

    if(timer == NULL){
        UTIL_TIMER_LOG("Invalid param: NULL timer \n");
        return -1;
    }

    for(item = g_timer_ctrl.act_timer_list; item != NULL; item = item->next){
        if(item == timer){
            /*update the next timer */
            if(item->next != NULL){
                item->next->offset_time += item->offset_time;
            }
            /*remove head*/
            if(prev == NULL){
                g_timer_ctrl.act_timer_list = item->next;
            }else{
                prev->next = item->next;
            }
            break;
        }
        prev = item;
    }

    /*not found */
    if(item == NULL){
        UTIL_TIMER_LOG("not found the timer \n");
        ret = -1;
    }
    return ret;
}

int util_timer_del(util_timer_t * timer)
{
    int ret = 0;

    if(timer == NULL){
        UTIL_TIMER_LOG("Invalid param: NULL timer \n");
        return -1;
    }

    cs_mutex_lock(g_timer_ctrl.mutex);

    if(0 == util_timer_act_remove(timer)){
        util_timer_free(timer);
    }else{
        ret = -1;
    }

    cs_mutex_unlock(g_timer_ctrl.mutex);

    return ret;
}

int util_timer_stop(util_timer_t * timer)
{
    int ret = 0;

    if(timer == NULL){
        UTIL_TIMER_LOG("Invalid param: NULL timer \n");
        return -1;
    }

    cs_mutex_lock(g_timer_ctrl.mutex);
    
    if(0 == util_timer_act_remove(timer)){
        /* move to stop list */
        timer->state = TIMER_STATE_STOP;
        timer->next = g_timer_ctrl.stop_timer_list;
        g_timer_ctrl.stop_timer_list = timer;
    }else{
        ret = -1;
    }

    cs_mutex_unlock(g_timer_ctrl.mutex);
    return ret;
}



int util_timer_restart(util_timer_t * timer)
{
    util_timer_t * item = NULL;
    util_timer_t * prev = NULL;
    int ret = 0;

    if(timer == NULL){
        UTIL_TIMER_LOG("Invalid param: NULL timer \n");
        return -1;
    }

    cs_mutex_lock(g_timer_ctrl.mutex);
    prev = NULL;
    for(item = g_timer_ctrl.stop_timer_list; item != NULL; item = item->next){
        if(item == timer){
            /*remove from stop list*/
            if(prev != NULL){
                prev->next = item->next;
            }else{
                g_timer_ctrl.stop_timer_list = item->next;
            }

            item->offset_time = item->timeout;
            item->state = TIMER_STATE_ACTIVE;
            item->next = NULL;
            util_timer_insert(item);
            break;
        }
        prev = item;
    }

    /*not found */
    if(item == NULL){
        UTIL_TIMER_LOG("not found the timer \n");
        ret = -1;
    }

    cs_mutex_unlock(g_timer_ctrl.mutex);
    return ret;
}


void util_timer_process()
{
    util_timer_t * item = NULL;
    unsigned long long start = util_os_current_time();
    
    item = g_timer_ctrl.act_timer_list ; 
    
    /*empty list*/
    if(item == NULL){
        return;
    }

    cs_mutex_lock(g_timer_ctrl.mutex);

    item->offset_time -= UTIL_TIMER_MIN_INTVAL;
    while(item != NULL && item->offset_time <= 0){
        if(item->callback != NULL){
            item->callback(item->data);
        }

        g_timer_ctrl.act_timer_list = item->next;

        if(item->type == TIMER_TYPE_CYCLE){
            item->offset_time = item->timeout;
            util_timer_insert(item);
        }else{
            util_timer_free(item); 
        }

        item = g_timer_ctrl.act_timer_list;
    }
    
    item = g_timer_ctrl.act_timer_list ; 
    if(item != NULL){
        item->offset_time -= util_os_current_time() - start;
    }

    cs_mutex_unlock(g_timer_ctrl.mutex);
}

void util_timer_thread()
{
    g_timer_ctrl.thread_id = util_os_thread_id();
    g_timer_init = 1;
    while(1){
        util_timer_process();
        util_os_timer_delay(UTIL_TIMER_MIN_INTVAL);
    }
}


void util_timer_show()
{
    util_timer_t * timer;
    cs_mutex_lock(g_timer_ctrl.mutex);
    printf("%-6s\t%-7s\t%-8s\t%-8s\n","TYPE","STATE","TIMEOUT","OFFSET");
    printf("%-6s\t%-7s\t%-8s\t%-8s\n","======","=======","========","========");
   
    timer = g_timer_ctrl.act_timer_list;
    while(timer != NULL){
        printf("%-6s\t%-7s\t%-8d\t%-8d\n",
                timer->type == TIMER_TYPE_ONCE? "ONCE":"CYCLE",
                timer->state == TIMER_STATE_STOP? "STOP":"ACTIVE",
                timer->timeout,
                timer->offset_time);
        timer = timer->next;
    }

    timer = g_timer_ctrl.stop_timer_list;
    
    while(timer != NULL){
        printf("%-6s\t%-7s\t%-8d\t%-8d\n",
                timer->type == TIMER_TYPE_ONCE? "ONCE":"CYCLE",
                timer->state == TIMER_STATE_STOP? "STOP":"ACTIVE",
                timer->timeout,
                timer->offset_time);
        timer = timer->next;
    }

    printf("\n\n");
    cs_mutex_unlock(g_timer_ctrl.mutex);
}   

