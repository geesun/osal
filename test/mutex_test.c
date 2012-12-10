#include <stdio.h>
#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>

#include "osal_api.h"

unsigned int mutex_id ;



void test_mutex_res(int a )
{
    int i = 0 ;
    printf("enter a = %d \n",a);
    cs_mutex_lock(mutex_id);
    while(i < 10){
        printf("a = %d \n",a);
        i++;
        sleep(1);
    }
    osal_mutex_info_show();
    cs_mutex_unlock(mutex_id);
    printf("exit a = %d \n",a);
}



void test_mutex_res2(int a )
{
    int i = 0 ;
    printf("enter bbb = %d \n",a);
    cs_mutex_lock(mutex_id);
    while(i < 10){
        printf("bbb = %d \n",a);
        i++;
        sleep(1);
    }
    osal_mutex_info_show();
    cs_mutex_unlock(mutex_id);
    printf("exit bbb = %d \n",a);
}


void *thread_main(void *arg)
{
    printf("thread main \n");
    while(1){
        test_mutex_res2(100);
        sleep(1);
    };
}

void test_create_max_mutex()
{
    unsigned int id ; 
    osal_mutex_sys_init(5);
    int i = 0; 

    for(; i < 10; i++){
         cs_mutex_init(&id, NULL,0);
    }
    osal_mutex_info_show();
    osal_mutex_sys_exit();
}

void test_mutex_not_exist()
{
    osal_mutex_sys_init(5);

    cs_mutex_lock(2);
    cs_mutex_lock(3);
    cs_mutex_lock(4);
    cs_mutex_unlock(4);
    cs_mutex_unlock(7);

    osal_mutex_info_show();
    osal_mutex_sys_exit();
}

void test_mutex_unlock_first()
{
    unsigned int id ; 
    osal_mutex_sys_init(5);

    cs_mutex_init(&id, NULL,0);
    printf("id = %d \n",id);
    cs_mutex_unlock(id);

    osal_mutex_info_show();
    osal_mutex_sys_exit();
}


void test_mutex_right_process()
{
    pthread_t pthread;
    pthread_attr_t attr;
    int ret = 0;

    osal_mutex_sys_init(5);
    
    cs_mutex_init(&mutex_id, NULL,0);

    pthread_attr_init(&attr);

    ret = pthread_create(&pthread, &attr, thread_main, NULL);

    while(1){
        test_mutex_res(1);
        sleep(1);
    };
}


void test_mutex()
{
    test_create_max_mutex();
    test_mutex_not_exist();
    test_mutex_unlock_first();

    test_mutex_right_process();
}
