#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <assert.h>

// 线程调度策略控制
// 获取线程的策略
static int get_thread_policy (pthread_attr_t *attr)
{
    int policy ;

    // 得到线程优先级
    int rs = pthread_attr_getschedpolicy(attr, &policy);
    // 表达式为假，向STDERR打印错误信息，调用abort终止调用    
    assert( 0 == rs);

    switch (policy)
    {
        case SCHED_FIFO:
            printf("SCHED is FIFO\n");
        break;
        case SCHED_OTHER:
            printf("SCHED is OTHER\n");
        break;
        case SCHED_RR:
            printf("SCHED is RR\n");
        break;
        default:
            printf("SCHED is Unkown %d\n", policy);
        break;
    }

    return policy;
}

// 展示线程的优先级 最大值最小值
static int show_thread_priority(pthread_attr_t *attr, int policy)
{
    int priority = sched_get_priority_max(policy);
    //assert( -1 == priority);
    printf("show_thread_priority max is %d\n", priority);
    priority = sched_get_priority_min(policy);
    //assert( -1 == priority);
    printf("show_thread_priority min is %d\n", priority);

}

// 获取线程的优先级
static int get_thread_priority(pthread_attr_t *attr)
{
    struct sched_param param;
    int rs = pthread_attr_getschedparam(attr, &param);
    assert( 0 == rs);

    return param.__sched_priority;
}

// 创建线程的优先级
static void set_thread_policy(pthread_attr_t *attr, int policy)
{
    int rs = pthread_attr_setschedpolicy(attr, policy);
    assert (0 == rs);

    get_thread_policy(attr);
}

int main()
{

    // 线程属性结构变量
    // 线程分离，线程调度，线程位置，线程大小，线程作用域，线程缓冲区
    pthread_attr_t attr;
    // 描述调度参数的结构
    struct sched_param sched;

    int rs;
    // 判断线程初始化是否成功
    rs = pthread_attr_init(&attr);
    assert (0 == rs);
    
    int policy = get_thread_policy(&attr);
    printf("output current configureation of priority \n");
    show_thread_priority(&attr, policy);
    printf("output SCHED_FIFO of priority\n");
    show_thread_priority(&attr, SCHED_FIFO);
    printf("output SCHED_RR of priority\n");
    show_thread_priority(&attr, SCHED_RR);
    printf("output SCHED_NOW of priority\n");
    // 获得线程优先级
    int priority = get_thread_priority(&attr);
    // 设置调度策略
    set_thread_policy(&attr, SCHED_FIFO);
    set_thread_policy(&attr, SCHED_RR);
    // 还原当前策略
    set_thread_policy(&attr, policy);


    // 销毁线程属性
    rs = pthread_attr_destroy(&attr);
    assert(0 == rs);
    return 0;
}


// 编译 gcc sched.c -o sched -lpthread