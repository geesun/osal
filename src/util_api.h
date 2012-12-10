#ifndef __UTIL_API_H__
#define __UTIL_API_H__

typedef enum{
    TIMER_TYPE_ONCE,
    TIMER_TYPE_CYCLE
}util_timer_type_t;

#define CS_INVALID_TIMER    0

typedef struct util_timer util_timer_t;

typedef  void (util_timer_handler_t)(void * data);


int util_timer_sys_init(unsigned int max_timer_num);
void util_timer_sys_exit();

util_timer_t *  util_timer_add(
        util_timer_type_t type,
        unsigned int timeout,
        util_timer_handler_t * callback,
        void * data);
int util_timer_del(util_timer_t * timer);
int util_timer_stop(util_timer_t * timer);
int util_timer_restart(util_timer_t * timer);


#define cs_timer_add(timeout,func,data)               (unsigned int) util_timer_add(TIMER_TYPE_ONCE,timeout,func,data)
#define cs_circle_timer_add(timeout,func,data)        (unsigned int) util_timer_add(TIMER_TYPE_CYCLE,timeout,func,data)
#define cs_timer_del(time_id)                          util_timer_del((util_timer_t *)time_id)

#endif
