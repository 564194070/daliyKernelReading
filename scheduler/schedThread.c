#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void threadfun0()
{
    sleep(1);
    int policy;
    // 这个数据结构里面就有一个线程优先级 描述调度的参数
    struct sched_param param;

    pthread_getschedparam(pthread_self(), &policy, &param);

    if (policy == SCHED_OTHER)
    {
        printf("SCHED_OTHER\n");
    }
    if (policy == SCHED_FIFO)
    {
        printf("SCHED_FIFO\n");
    }
    if (policy == SCHED_RR)
    {
        printf("SCHED_RR\n");
    }

    for (int index = 1; index < 10; ++index)
    {
        for (int level = 1; level < 40000; ++level)
        {

        }
        printf("pthreadfunc000000000000000000000000 EXIT\n");
    }
    printf("pthreadfunc00000000000000000000000000000\n");
}

void threadfun1()
{
    sleep(1);
    int policy;
    // 这个数据结构里面就有一个线程优先级 描述调度的参数
    struct sched_param param;

    pthread_getschedparam(pthread_self(), &policy, &param);

    if (policy == SCHED_OTHER)
    {
        printf("SCHED_OTHER\n");
    }
    if (policy == SCHED_FIFO)
    {
        printf("SCHED_FIFO\n");
    }
    if (policy == SCHED_RR)
    {
        printf("SCHED_RR\n");
    }

    for (int index = 1; index < 10; ++index)
    {
        for (int level = 1; level < 40000; ++level)
        {

        }
        printf("pthreadfunc111111111111111111111111111111\n");
    }
    printf("pthreadfunc111111111111111111111111111111 EXIT\n");
}

void threadfun2()
{
    sleep(1);
    int policy;
    // 这个数据结构里面就有一个线程优先级 描述调度的参数
    struct sched_param param;

    pthread_getschedparam(pthread_self(), &policy, &param);

    if (policy == SCHED_OTHER)
    {
        printf("SCHED_OTHER\n");
    }
    if (policy == SCHED_FIFO)
    {
        printf("SCHED_FIFO\n");
    }
    if (policy == SCHED_RR)
    {
        printf("SCHED_RR\n");
    }

    for (int index = 1; index < 10; ++index)
    {
        for (int level = 1; level < 40000; ++level)
        {

        }
        printf("pthreadfunc222222222222222222222222222222\n");
    }
    printf("pthreadfunc222222222222222222222222222222 EXIT\n");
}

int main(int argc, char *argv[])
{
    // 获取当前的用户识别码
    int uid = getuid();

    if (0 == uid)
    {
        // 用户是root特权
        printf("the current user is root. \n");
    }
    else 
    {
        //用户是普通用户
        printf("the current user is not root!\n");
    }


    // 设置三个线程
    pthread_t ppid1, ppid2, ppid3;
    struct sched_param param;


    pthread_attr_t attr1,attr2,attr3;
    pthread_attr_init(&attr1);
    pthread_attr_init(&attr2);
    pthread_attr_init(&attr3);

    // 设置优先级
    param.__sched_priority = 61;

    //设置线程调度策略
    pthread_attr_setschedpolicy(&attr1, SCHED_RR);
    pthread_attr_setschedpolicy(&attr2, SCHED_FIFO);
    pthread_attr_setschedpolicy(&attr3, SCHED_OTHER);

    pthread_attr_setschedparam(&attr1, &param);
    pthread_attr_setschedparam(&attr2, &param);
    pthread_attr_setschedparam(&attr3, &param);


    pthread_attr_setinheritsched(&attr1, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setinheritsched(&attr2, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setinheritsched(&attr3, PTHREAD_EXPLICIT_SCHED);


    pthread_create(&ppid1, &attr1, (void*)threadfun0, NULL);
    pthread_create(&ppid2, &attr2, (void*)threadfun0, NULL);
    pthread_create(&ppid3, &attr3, (void*)threadfun0, NULL);

    pthread_join(ppid3, NULL);
    pthread_join(ppid2, NULL);
    pthread_join(ppid1, NULL);

    pthread_attr_destroy(&attr3);
    pthread_attr_destroy(&attr2);
    pthread_attr_destroy(&attr1);

    return 0;
}