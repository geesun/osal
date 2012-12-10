#ifndef __UTIL_TIMER_OS_H__
#define __UTIL_TIMER_OS_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>


typedef pthread_t  util_thread_id_t;

#define   util_os_thread_id    pthread_self
unsigned long long util_os_current_time();

void util_os_timer_delay(int ms);
void util_os_timer_thread_start(const void * func);
#endif


