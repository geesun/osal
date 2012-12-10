#ifndef __UTIL_TIMER_OS_H__
#define __UTIL_TIMER_OS_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cyg/kernel/kapi.h>

typedef cyg_handle_t   util_thread_id_t;

#define   util_os_thread_id         cyg_thread_self
#define   util_os_current_time      cyg_current_time

void util_os_timer_delay(int ms);
void util_os_timer_thread_start(const void * func);
#endif
