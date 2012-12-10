#include <stdio.h>
#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>

#include "util_api.h"

unsigned int  data[10];
unsigned int  item_data;

void test_timer_proc(void * data)
{
    int  * num = (int *) data;
    printf("timer out %d \n",*num);
}


void test_one_once_timer()
{
    cs_timer_add(5000,test_timer_proc, &data[0]);
}


void test_one_cycle_timer()
{
    cs_circle_timer_add(1000,test_timer_proc, &data[1]);
}


void test_small_to_larger()
{
    int i = 0; 

    for( i = 0; i < 10; i++){
        cs_timer_add(data[i] * 1000,test_timer_proc, &data[i]);
    }
}


void test_large_to_small()
{
    int i = 0; 

    for( i = 9; i >=0; i--){
        cs_timer_add(data[i] * 1000,test_timer_proc, &data[i]);
    }
}

void test_duplicate_timer()
{
    int i = 0; 

    for( i = 0; i <10; i++){
        cs_timer_add(data[i] * 1000,test_timer_proc, &data[i]);
    }

    item_data = 23;
    for( i = 9; i >=0; i--){
        cs_timer_add(5 * 1000,test_timer_proc, &item_data);
    }
}

void test_running_insert_timer()
{
    cs_timer_add(5000,test_timer_proc, &data[0]);
    sleep(1); 
    util_timer_show();
    cs_timer_add(2000,test_timer_proc, &data[1]);
    util_timer_show();
    sleep(2);
    util_timer_show();
    cs_timer_add(1000,test_timer_proc, &data[2]);
    util_timer_show();
}


void test_stop_restart_timer()
{
    /*to avoid 64bit system issue, use util_timer_add*/
    util_timer_t * timer; 
    util_timer_t * timer2; 
    util_timer_t * timer3; 

    cs_timer_add(4000,test_timer_proc, &data[1]);

    timer = util_timer_add(TIMER_TYPE_ONCE,10000,test_timer_proc, &data[2]);
    timer2 = util_timer_add(TIMER_TYPE_ONCE,2000,test_timer_proc, &data[3]);
    timer3 = util_timer_add(TIMER_TYPE_ONCE,3000,test_timer_proc, &data[4]);
    util_timer_show();
    sleep(1);
    
   // util_timer_stop(timer);
    //util_timer_show();
    util_timer_stop(timer2);
    util_timer_show();
    util_timer_stop(timer3);

    util_timer_show();
    sleep(10);  /*wait for timeout, timer will not exe*/
    
    util_timer_show();
   // util_timer_restart(timer);  /*time will exe after 5s*/
    //util_timer_restart(timer2);  /*time will exe after 5s*/
    util_timer_restart(timer3);  /*time will exe after 5s*/
    util_timer_show();

    sleep(5);
}


void test_timer_delete_proc(void * data)
{
    int  * num = (int *) data;
    printf("delete timer out %d \n",*num);
    sleep(5);  /*WAIT AND LET DELETE IT*/
    printf("delete exit %d \n",*num);
}


void test_delete_timer()
{
    /*to avoid 64bit system issue, use util_timer_add*/
    util_timer_t * timer; 
    util_timer_t * timer2; 
    util_timer_t * timer3; 


    timer = util_timer_add(TIMER_TYPE_ONCE,1000,test_timer_delete_proc, &data[2]);
    timer2 = util_timer_add(TIMER_TYPE_ONCE,2000,test_timer_delete_proc, &data[3]);
    timer3 = util_timer_add(TIMER_TYPE_ONCE,3000,test_timer_delete_proc, &data[4]);
    util_timer_show();
    
    //sleep(2);

    printf("start delete the timer \n");

    /*seperate test below*/

    /*delete header */
    util_timer_del(timer);
    util_timer_show();
    /*delete mid */
    util_timer_del(timer2);
    util_timer_show();
    /*delete tail */
    util_timer_del(timer3);
    util_timer_show();
    


    /*delete don't exist timer*/
    util_timer_del(timer3);
    printf("end delete the timer \n");
    
}



void test_timer_add_proc(void * data)
{
    int  * num = (int *) data;

    printf("add another timer %d \n",*num);
    cs_timer_add(5000,test_timer_add_proc, &data[0]);
}


void test_timer_add_in_timerout()
{
    cs_timer_add(5000,test_timer_add_proc, &data[0]);

}

void  test_timer_thread()
{
    int i = 0; 

    for( i = 0; i < 10; i++){
        cs_timer_add(data[i] * 1000,test_timer_proc, &data[i]);
    }
    util_timer_show();
}

void test_more_thread_add_delete()
{
    pthread_t pthread;
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    pthread_create(&pthread, &attr, test_timer_thread, NULL);
    pthread_create(&pthread, &attr, test_timer_thread, NULL);

    int i = 0; 
    sleep(1);
    for( i = 0; i < 10; i++){
        cs_timer_add(data[i] * 1000,test_timer_proc, &data[i]);
    }
    util_timer_show();

}


void test_timer()
{
    int i;

    for( i = 0; i < 10; i++){
       data[i] = i; 
    }

    util_timer_sys_init(50);
    sleep(2);
    //test_one_once_timer();
    
    //test_one_cycle_timer();
    //
    //test_small_to_larger();
    //sleep(1);
    //test_large_to_small();
    //sleep(1);
    //test_duplicate_timer();
    //test_running_insert_timer();
    //
    //test_stop_restart_timer();
    //test_delete_timer();
    //test_timer_add_in_timerout();
    test_more_thread_add_delete();
}


