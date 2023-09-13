# 调度器

# 1.调度器分析
# 1.1 调度器
- 内核中安排进程执行的模块 scheduler
- 切换进程状态 process state 执行/可中断睡眠/不可中断睡眠/退出/暂停
- 调度器负责管理CPU
1. 选择某些就绪进程来执行
2. 打断正在执行的程序，让他们变为就绪态
3. 抢占式调度器：支持就绪态和执行态双向切换。有些调度器切换方向是单向的，只能从就绪态切换到执行态，不能把执行中的程序变成就绪态。
4. 每当一个程序变为就绪态时，马上调度另一个程序开始执行，交替使用CPU

- 就绪：获得了除了CPU之外的所有资源，等到CPU就立即执行
- 运行：获得CPU了，程序正在执行
- 阻塞：等待某个事件，无法执行，放弃CPU了。事件触发后，硬件发送信号，进程从阻塞态恢复到就绪态。接收SIGSTOP信号，也会切换到阻塞状态

[上下文切换] CPU中进程切换执行过程 context switch 将进程在CPU中切换执行的过程，内核承担此任务，负责重建和存储被切换掉之前的CPU状态

# 1.2 调度类sched\_class结构体

- sched\_class: 结构体标识调度类
- 源码位置: kernel/sched/sched.h

- struct sched\_class
01. const struct sched\_class \*next  
- 系统中有多个调度类，按照优先级排成了一个链表，下一个优先级调度类，就是指针指向
02. void (\*enqueue\_task) (struct rq \*rq, struct task\_struct \*p, int flags)
- 将进程加入到就绪队列中去，将进程实体(PCB控制块 task\_struct) 存放到红黑树当中(rq) 并对nr\_running变量自动+1
- 一个task变为就绪状态，希望挂在调度器就绪队列上(rq)
03. void (\*dequeue\_task) (struct rq \*rq, struct task\_struct \*p, int flags)
- 从执行队列删除进程 并且对nr\_running变量自动-1 将调度实体从红黑树中移除
- 一个task阻塞等待输入，不能继续停留在调度器就绪队列了，要离开
- 某个task推出可运行状态时也会调用
04. void (\*yield\_task) (struct rq \*rq)
- 放弃CPU执行权，该函数执行先出队再入队，直接将调度实体(进程)放到红黑树最右端
- 跳过当前任务,进程想要放弃对处理器的控制权
05. bool (\*yield\_to\_task) (struct rq \*rq, struct task\_struct \*p, bool preempt)
- 跳过当前任务，并且尽量调度任务p
06. void (\*check\_preempt\_curr) (struct rq \*rq, struct task\_struct, int flags)
- 这个函数在有任务被唤醒的时候调用，看看能不能抢占当前调用
- 当前进程可否被新进程抢占
- 完成抢占之前CFS会进行公平性测试
07. struct task\_struct \* (\*pick\_next\_task) (struct rq \*rq, struct task\_struct \*prev)
- 选择下一个就绪要运行的进程
08. void (\*put\_prev\_task) (struct rq \*rq, struct task\_struct\*p)
- 将进程放回到运行队列当中去
- 用于另一个进程代替当前运行进程
09. int (\*select\_task\_rq) (struct task\_struct\*p, int task\_cpu, int sd\_flag, int flags)
- 为进程选择一个合适的CPU
10. void (\*migrate\_task\_rq) (struct task\_struct \*p)
- 迁移任务到其他CPU
11. void (\*task\_waking) (struct task\_struct \*task)
12. void (\*task\_woken) (struct rq \*this\_rq, struct task\_struct \*task)
- 唤醒进程
13. void (\*set\_cpus\_allowed) (struct task\_struct \*p, const struct cpumask \*newmask)
- 修改进程在CPU的亲和性
14. void (\*rq\_online) (struct rq \* rq)
- 启动运行队列
15. void (\*rq\_offline) (struct rq \* rq)
- 停止运行队列
16. void (\*set\_curr\_task) (struct rq \*rq)
- 当进程改变它的调度类或者进程组 会引起调用
17. void (\*task\_tick) (struct rq \*rq, struct task\_struct \*p, int queued)
- 他可能引起进程切换，将驱动运行时抢占
- 用作时钟中断的回调
18. void (\*task\_fork) (struct task\_struct \*p)
- 当进程创建的时候调用，不同调度策略进程初始化方式不同
19. void (\*task\_dead) (struct task\_struct \*p)
- 当进程退出时调用
20. void (\*switched\_from) (struct rq \* this\_rq, struct task\_struct \*task)
21. void (\*switched\_to) (struct rq \* this\_rq, struct task\_struct \*task)
- 用于进程切换操作 
22. void (\*prio\_changed) (struct rq \*this\_rq, struct task\_struct \*task, int oldprio)
- 更改进程优先级



- Linux中有很多优先调度类，他们可以组合成一个链表，下一级优先调度类

# 1.3调度类（优先级从高到低）
Linux 选择调度类的优先级顺序
源码位置: /kernel/sched/sche\_task
- 每一个进程对应一种调度策略，每一个调度策略对应一个调度类
- 每个调度类可以对应多个调度策略，每个调度策略可以对应多个进程
- 更具需求，将不同的任务放到不同的调度类当中

1. stop\_sched\_class
- 优先级最高的线程，会中断所有其他线程，而且不会被其他任务打断 
- 不能被抢占不能被切换，不会主动yield,直到run出CPU
- (stop\_task)
2. dl\_sched\_class 
-
- deadline
3. rt\_sched\_class (FIFO RR)
- 实时调度器:以优先级为导向，选择优先级最高的进程运行
- 作用于实时线程
- rt
- SCHED\_RR SCHED\_FIFO 关联
4. fair\_sched\_class (NORMAL BATCH)
- 完全公平调度器(CFS): 管理一般常规线程
- fair
- SCHED\_NORMAL SCHED\_BATCH SCHED\_IDLE 相关联
5. idle\_sched\_class 
- 静态线程调度器
- 每个CPU第一个PID=0的线程，swapper是一个静态线程，一般会在开机和CPU异常的时候做DUMP
- idle\_task


# 1.4 调度策略
优先级从高至底
源码: /include/uapi/linux/sched.h
影响下一个进程的选择 \*pick\_next\_task, 进程中使用unsigned int policy保存进程调度策略


1. SCHED\_DEADLINE   6
- 实时进程调度策略，针对突发性计算。时间高敏感度。
2. SCHED\_FIFO	     1	
- 先入先出调度算法，实时调度策略，先到先服务，高优先级任务可以直接抢占低优先级任务
3. SCHED\_RR         2
- 轮流调度算法，实时调度策略，采用时间片，相同优先级的任务执行完放到尾部队列保证公平，高优先级会抢占低优先级任务
4. SCHED\_NORMAL     0
- 普通进程调度策略，使用CFS调度器实现
5. SCHED\_BATCH      3
- SCHED\_NORMAL分化版本，采用分时测率实现，根据动态优先级，分配CPU运行资源
- 非交互处理器，消耗性进程
6. SCHED\_IDLE       5
- 优先级最低，系统空闲的时候，才运行该类进程
7. SCHED\_ISO        4

# 1.5 Linux调度顺序
Linux 调度核心选择下一个合适的task运行时，会按照优先级顺序遍历调度类的pick\_next\_task函数。



