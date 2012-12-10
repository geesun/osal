#include "util_timer_os.h"


unsigned long long util_os_current_time()
{
    struct timeval tv;
    unsigned long long time = 0;
    unsigned long long s_to_tick = 0;

    s_to_tick = sysconf(_SC_CLK_TCK);
    gettimeofday(&tv, NULL);
    time = (unsigned long long )(tv.tv_sec*s_to_tick + (tv.tv_usec/1000)/s_to_tick);
    return time;
}

void util_os_timer_delay(int ms)
{
   struct timespec ts;

   ts.tv_sec = (ms/1000);
   ts.tv_nsec = (ms - ts.tv_sec * 1000) * 1000000;
   nanosleep (&ts, NULL);
}

void util_os_timer_thread_start(const void * func)
{   
    pthread_t pthread;
    pthread_attr_t		custom_attr;
    pthread_attr_init(&custom_attr);
    pthread_create(&pthread,
            &custom_attr,
            func,
            NULL);
}
