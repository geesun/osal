#ifndef __UTIL_TIMER_H__
#define __UTIL_TIMER_H__
#include "util_api.h" 

#define UTIL_TIMER_MIN_INTVAL    10  /*10 ms*/

typedef enum{
    TIMER_STATE_STOP,
    TIMER_STATE_ACTIVE
}util_timer_state_t;


#define UTIL_INVALID_TIMER   CS_INVALID_TIMER 

struct util_timer{
    unsigned char         valid;
    util_timer_type_t     type;
    util_timer_state_t    state;

    unsigned int          timeout;
    int                   offset_time;

    util_timer_handler_t  *callback;
    void                  *data;
    struct util_timer     *next;
};

typedef struct{
    util_timer_t    * act_timer_list;
    util_timer_t    * stop_timer_list;
    util_timer_t    * time_data;
    unsigned int      max_timer_num;
    unsigned int      mutex;
    util_thread_id_t  thread_id;
}util_timer_ctrl_t;



#define UTIL_TIMER_LOG(arg,...) \
    printf("%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define UTIL_TIMER_LOG_INFO(arg,...) \
    printf("%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define UTIL_TIMER_LOG_MIN(arg,...) \
    printf("%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define UTIL_TIMER_LOG_MAJ(arg,...) \
    printf("%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define UTIL_TIMER_LOG_CRI(arg,...) \
    printf("%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define UTIL_TIMER_DEBUG(arg,...) \
    printf("%s(): "arg, __FUNCTION__,##__VA_ARGS__)




#define UTIL_TIMER_MALLOC(size)        malloc((size))
#define UTIL_TIMER_FREE(ptr)           if((ptr) != NULL){ free((ptr)); (ptr) = NULL; }

#endif
