# SMP

# 1.处理器
## 1.1处理器分类
多处理器系统工作方式分类
1. 非堆成多处理器(asym-metrical mulit-processing)
2. 对称多处理器 (symmetrical mulit-processing)
- 所有处理器，位置相同
- 存储器，中断。IO具有相同可访问性，消除结构障碍


## 1.2内核和多处理器结合的问题
Linux SMP调度 就是将进程安排/迁移到合适的CPU上去，保持各CPU负载均衡的过程
1. CPU符合必须尽可能公平的在所有处理器上共享
2. 进程与系统中某些处理器的亲和性是可设置的
3. 内核必须能够将一个进程在不同CPU间迁移

## 1.3SMP优点
1. 增加吞吐量的 高性价比方案
2. 由于操作系统由所有处理器共享，他们提供了单独的一个系统映像(容器管理)
3. 对于单独的问题应用多处理器，并行编程
4. 负载均衡由操作系统实现
5. 单处理器(Up)编程模型可用于一个SMP当中
6. 对共享数据，他们具有伸缩性
7. 所有数据可以由所有处理器寻址，由硬件监视逻辑保持连续性
8. 由于通信全局共享，内存执行。处理器之间通信，不需要使用消息传送库完成。

## 1.4.SMP局限性
1. 由于高速缓存相关性，锁定机制，共享对象和其他问题。可伸缩性收到限制
2. 需要新技术利用多处理器，线程编程/设备驱动

# 2.处理器数据结构
## 2.1 CPU域初始化
使用CPU域来描述CPU的层次关系，内核使用bitmap管理CPU
- 源码 /include/linux/sched.h

- struct sched\_domain\_topology\_level
- sched\_domain\_mask\_f mask         函数指针 指定某个SDTL层级的cpumask位置
- sched\_domain\_flags\_f sd\_flags   函数指针 指定某个SDTL层级的标志位.


## 2.2 内核管理CPU
通过bitmap达到管理效果
源码: /kernel/cpu.c
1. possible
- const struct cpumask \*const cpu\_possible\_mask = to\_cpumask(cpu\_possible\_bits)
- EXPORT\_SYMBOL(cpu\_possible\_mask)
- 可运行CPU核心的数目
2. present
- static DECLARE\_BITMAP(cpu\_online\_bits, CONFIG\_NR\_CPUS) \_\_read\_mostly
- const struct cpumask \*const cpu\_conline\_mask = to\_cpumask(cpu\_online\_bits)
- EXPORT\_SYMBOL(cpu\_online\_mask)
- 表示系统中有多少个具online条件的CPU核心，不一定都处于online状态，热插拔
3. online
- static DECLARE\_BITMAP(cpu\_present\_bits, CONFIG\_NR\_CPUS) \_\_read\_mostly
- const struct cpumask \*const cpu\_present\_mask = to\_cpumask(cpu\_present\_bits)
- EXPORT\_SYMBOL(cpu\_present\_mask)
- 表示系统中有多少个正在处运行状态的CPU
4. active
- static DECLARE\_BITMAP(cpu\_active\_bits, CONFIG\_NR\_CPUS) \_\_read\_mostly
- const struct cpumask \*const cpu\_active\_mask = to\_cpumask(cpu\_active\_bits)
- EXPORT\_SYMBOL(cpu\_active\_mask)
- 表示系统当中有多少活跃的CPU核心

以上都是bitmap类型标识，每一位代表一个CPU
CONFIG\_NR\_CPUS 系统最大CPU核心数量

## 2.3SMP负载均衡
从注册软中断开始，每次系统处理调度tick时，都会检查当前是否需要处理SMP负载均衡
每个CPU对应一个可以执行的队列，进程task\_run 他就会被加入到一个可执行队列当中去（持续task\_run的进程 总是趋于同一个CPU上，有利于缓存）
CPU之访问自己的调度队列，避免竞争

1. 周期性调用进程scheduler\_tick()-> trigger\_load\_balance() 通过软中断触发负载均衡
- 周期性负载均衡，CPU对应的运行队列数据结构，记录下一次周期性负载均衡的时间
- 超过这个时间点后，直接触发SCHED\_SOFTIRQ，通过这个软中断触发负载均衡
- scheduler\_tick()-> trigger\_load\_balance()
2. 某个CPU上无可运行进程，\_\_schedule() 准备调度idle进程前，会尝试从其他CPU拉取进程执行
3. 将进程A从一个CPU运行队列，迁移到另一个CPU运行队列

# 3 CPU调度域逻辑关系
2路4核8核心CPU
- 暂不展开
- NUNA-> MC -> SMT
1. SMT 超线程
- 一个物理核心，可以执行两个线程，就叫做超线程
2. MC 多核
-
3. NUMA
-
4. 进程负载均衡尽量在底层调度域内部完成，提高了缓存利用率


# 4.总结
分层角度，所有CPU分为3层(SMT-> MC-> NUMA)
- 每层包含了所有物理CPU，但是我们划分粒度不同，根据高速缓存和内存的相关性，划分调度域。
- 调度域内CPU划分为调度组，越下层，调度域越小。

用到SMP负载均衡的时机
1. 内核运行中，需要用SMP负载均衡模型，确定最佳运行CPU
- A进程唤醒B进程，try\_to\_wake\_up会考虑进程B运行在那个CPU上
- 进程调用execve
- fork出子进程，第一次被调度运行时

Linux运行时调优
引入了sysctl在运行时，对调度程序调优，单位以纳秒为主
1. sched\_child\_runs\_first 
- 在fork之后进行调度，设置为0会先调度父进程
2. sched\_min\_granularity\_ns 
- 针对CPU密集型任务执行最低级别抢占粒度
3. sched\_latency\_ns 
- 针对CPU密集型任务进行目标抢占延迟
4. sched\_stat\_granularity
- 收集调度程序统计信息的粒度
