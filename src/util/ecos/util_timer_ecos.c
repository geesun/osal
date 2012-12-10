#include "util_timer_os.h"

#define UTIL_OS_TIMER_STACK_SIZE     (8*1024)

unsigned char     g_stack_buf[UTIL_OS_TIMER_STACK_SIZE];

void util_os_timer_delay(int ms)
{
    unsigned int sys_ticks;
    sys_ticks = ms/10;
    
    if(sys_ticks == 0){
        sys_ticks = 1;
    }

    cyg_thread_delay(sys_ticks);
}


void util_os_timer_thread_start(const void * func)
{
    cyg_handle_t  thread_id;
    cyg_thread    thread_ctrl;
    cyg_thread_create(5,
                      (cyg_thread_entry_t  *)func,
                      (cyg_addrword_t)0,
                      "TIMER_TASK"
                      g_stack_buf,
                      UTIL_OS_TIMER_STACK_SIZE,
                      &thread_id,
                      &thread_ctrl);
    cyg_thread_resume(thread_id);
}
