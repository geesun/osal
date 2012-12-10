#include <stdio.h>
#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>

#include "osal_api.h"
#include "util_api.h"

extern void test_mutex();
extern void test_timer();

int main()
{
    //test_mutex();
    test_timer();

    while(1){
        util_timer_show();
        sleep(1);
    }

    return 0;
}
