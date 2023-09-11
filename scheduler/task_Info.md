# 1.task\_struct数据结构

# 1.1 数据结构基础信息
task\_struct 进程管理和调度的核心数据结构,和多个其他子系统有关联
位置: /include/linux/sched.h

# 1.2task\_struct 进程描述符，进程控制块PCB。
主要处理内核对进程的描述和管理工作
主要结构成员信息
- volatile long state 进程状态 -1:就绪态 0:运行态 >0:停止态
- void * stack 指向内核栈 指针
- automic\_t usage 有几个进程在使用此结构
- unsigned int flags 标识
- unsigned int ptrace 实现断点调试，跟踪进程运行
- #ifdef CONFIG\_SMP 条件编译-多处理器会用到此处
- #endif
- int on\_rq 运行队列和进程调度的相关程序
- int prio 动态优先级数值越小优先级越高 0-139 100-139时非实时进程 0-99时实时进程
- int static\_prio 静态优先级，只能通过系统调用去修改 static\_prio = MAX\_RT\_PRIO+nice+20 100-139
- int normal\_prio 归一化优先级，取决于静态优先级和调度策略，非实时进程归一化优先级等于静态优先级
- unsigned int rt\_priority 实时优先级，0-99，prio = MAX\_RT\_PRIO-1-p -> rt\_priority 实时优先级越大，优先程度越高
- const struct sched\_class * sched\_class 关于进程调度器相关
- #ifdef CONFIG\_CGROUP\_SCHED 
- struct task\_group * sched\_task\_group 结构体链表
- #ifdef CONFIG\_BLK\_DEV\_TO\_TRACE
- unsigned int btrace\_seq 块设备IO层跟踪工具
- unsigned int policy 进程调试策略相关字段
- int nr\_cpus\_allowed
- cpumask\_t cpus\_allowed
- #ifdef CONFIG\_PREEMPT\_RCU RCU同步原语
- struct list\_head tasks 进程架构链表
- struct mm\_struct \*mm, \*active_mm 进程管理进程地址空间，每个进程有独立地址空间，指向进程地址空间i
- task state 进程状态参数 
- int pdeath\_signal 接收父进程终止时，就会发出信号
- pid\_t pid 进程的PID
- pid\_t tgid 父进程tgid
- #ifdef CONFIG\_CC\_STACKPROTECTOR 防止内核堆栈溢出
- struct task\_struct \_\_rcu \*real\_parent 初始化父进程
- struct task\_struct \_\_ruc \*parent 接收终止进程
- struct list\_head children 子进程链表
- struct list\_head sibling 兄弟进程链表
- struct task\_struct \* group\_leader 线程组组长
- struct list\_head ptraced 关于断点调试的系统调用
- struct pid\_link pids[PIDTYPE\_MAX] PID-PID散列表的关系
- struct completion \*vfork\_done 实际上创建新进程的操作(fork,vfork,clone的最终归宿)
- cputime\_t utime, stime 用户态时间和内核态时间
- struct nameidata \*nameidata 文件系统信息
- struct sysv\_sem sysvsem 进程通讯空间
- struct sysv\_shm sysvshm
- ifdef CONFIG\_DETECT\_HUNG\_TASK 挂起任务检测
- unsigned long last\_switch\_count
- struct fs\_struct \*fs 文件系统信息
- u32 parent\_exec\_id 线程组
- u32 self\_exec\_id
- void \*journal\_info 系统日志文件系统信息
- struct bio\_list \*bio\_list 块设备链表
- VM state 虚拟内存状态
- struct reclaim\_state \*reclaim\_state 虚拟内存状态，内存回收
- struct backing\_dev\_info \*backing\_dev\_info 存放块设备IO流量信息
- struct io\_context \*io\_context IO调度器所用上下文信息
- struct task\_io\_accounting ioac 记录进程IO计数
- #ifdef CONFIG\_FUTEX futex同步机制
- #ifdef CONFIG\_PERF\_EVENTS 内存检测工具 Perfromance Event
- struct rcu\_head rcu RCU链表信息
- struct pipe\_inode\_info \*splice\_pipe 管道
- #ifdef CONFIG\_TASK\_DELAY\_ACCT 延迟计数











