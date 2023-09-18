# SMP

# 1.处理器
# 1.1处理器分类
多处理器系统工作方式分类
1. 非堆成多处理器(asym-metrical mulit-processing)
2. 对称多处理器 (symmetrical mulit-processing)
- 所有处理器，位置相同
- 存储器，中断。IO具有相同可访问性，消除结构障碍


# 1.2内核和多处理器结合的问题
Linux SMP调度 就是将进程安排/迁移到合适的CPU上去，保持各CPU负载均衡的过程
1. CPU符合必须尽可能公平的在所有处理器上共享
2. 进程与系统中某些处理器的亲和性是可设置的
3. 内核必须能够将一个进程在不同CPU间迁移

# 1.3SMP优点
1. 增加吞吐量的 高性价比方案
2. 由于操作系统由所有处理器共享，他们提供了单独的一个系统映像(容器管理)
3. 对于单独的问题应用多处理器，并行编程
4. 负载均衡由操作系统实现
5. 单处理器(Up)编程模型可用于一个SMP当中
6. 对共享数据，他们具有伸缩性
7. 所有数据可以由所有处理器寻址，由硬件监视逻辑保持连续性
8. 由于通信全局共享，内存执行。处理器之间通信，不需要使用消息传送库完成。

# 1.4.SMP局限性
1. 由于高速缓存相关性，锁定机制，共享对象和其他问题。可伸缩性收到限制
2. 需要新技术利用多处理器，线程编程/设备驱动

# 2.处理器数据结构
# 2.1 CPU域初始化
使用CPU域来描述CPU的层次关系，内核使用bitmap管理CPU
- 源码 /include/linux/sched.h

- struct sched\_domain\_topology\_level
- sched\_domain\_mask\_f mask         函数指针 指定某个SDTL层级的cpumask位置
- sched\_domain\_flags\_f sd\_flags   函数指针 指定某个SDTL层级的标志位.


# 2.2 内核管理CPU
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

