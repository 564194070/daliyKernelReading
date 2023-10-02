# 堆管理

# 1.堆
进程中主要用于动态分配变量和数据的内存区域，堆管理堆程序员来说，不是直接可见的。
- 依赖标准库提供的各个辅助函数(malloc/free)，来分配任意长度内存区
- malloc和内核之间的接口是brk系统调用，负责拓展/收缩堆
- 堆是一个连续的内存结构，拓展时自下至上增长，
- mm\_struct结构，包含堆在虚拟地址空间中起始(start\_brk)和当前结束地址(brk)

# 2.mm\_struct
源码位置: include/linux/mm\_types.h


# 3.brk 系统调用
用于指定堆在虚拟空间中新的结束地址，可以拓展或收缩，可以小于当前值
- brk系统调用通过do\_brk增长动态分配区
- 源码位置: mm/mmap.c
- 不是一个独立的内核概念，基于匿名映射实现，减少内部开销
- 检查过用作brk的新地址未超过限制，sys\_brk受限会将请求的地址按照页长对齐
- brk用于用户进程向内核申请空间，用于拓展或回收用户堆栈空间。
- brk用于大块空间申请， malloc为小块空间申请

# 4. sys\_brk 流程
1. 检查资源限制
2. 将brk值对齐到页
3. 是否想增加brk值
4. do\_brk 返回新的brk数值 增长动态分配区
5. do\_munmap 返回新的brk的值 释放动态分配区


# 5.do\_brk 源码流程分析
static  unsigned long do\_brk (unsigned long addr, unsigned long len)
向后继续申请动态分配区空间，将addr从地址位置，向后继续申请len字节长度，拓展堆内存长度
1. len = PAGE\_ALIGN(len) 
- 内存对齐
- 对len这个长度进行页面对齐
- 去判断页面对齐后是否超出边界，超出边界就不执行
2. get\_ynmapped\_area(NULL, addr, len, 0, MAP\_FIXED)
- 检查是否有足够的内存空间来分配len大小的内存
- 判断虚拟地址是否有足够的空间
- 和体系结构耦合
3. find\_vma\_links(mm, addr, addr+len, &prev, &rb\_link, &rb\_parent)
- 循环便利用户进程中红黑树的VMA，根据addr来查找合适的插入点
4. may\_expand\_vm(mm, len >> PAGE\_SHIFT) 
- 检查是否需要对虚拟区间进行扩充
5. security\_vm\_enough\_memory\_mm (mm, len >> PAGE\_SHIFT)
- 判断系统是否有足够的内存
6. vma\_merge
- 虚拟内存合并，合并成一个vam区
- 没办法合并，只有新创建一个VMA，地址空间时[addr, addr+len]
7. INIT\_LIST\_HEAD(&vma -> anon\_vma\_chain)
- 指向匿名域的指针


# 6.VMA
struct vm\_area\_struct
内核管理用户空间的数据结构
1. vm\_mm 指向VMA所属进程的struct mm\_struct
2. vm\_start 保存虚拟内存空间的首地址
3. vm\_end 保存虚拟内存空间的末地址后，第一个字节的地址
4. vm\_pgoff 指定文件映射的偏移量，单位为页面
5. vm\_flags 保存VMA的标志位
- VM\_EXEC 页可以被执行
- VM\_SHARED 页可以被多个进程共享
6. vm\_page\_prot
- VMA访问权限，用于创建区域中各页目录，页表，存取控制标志
7. vma\_link
- VMA结构体，将新的VMA添加到红黑树上面已经对应的链表上
8. 增加进程地址空间长度
out 

# 7.内存保护的过往

## 7.1 BKL 大内核锁
Big Kernel Lock ， 是递归锁(自旋锁不能递归)， 大内核锁保护内核，自旋锁保护共享资源
- lock\_kernel unlock\_kernel
- 同一进程可以对BKL嵌套，同一进程对BKL上锁和释放，当解锁次数=上锁次数，BKL才被真正释放
- BKL有自动释放特性 在CPUN上，进程A持有BKL，CPUN发生调度，进程持有的BKL被自动释放
- 与普通锁原理基本一致，一旦进程获取BKL，则进入被保护的临界区，临界区和所有被保护的临界区一起锁住
- 一般用于驱动和文件系统，用于支持SMP架构，一个处理器进入内核态就立刻上锁
- 多处理器性能，只能体现在用户态并行处理上，内核态还是单处理器执行
- 2.6.39后被淘汰 保持锁时间太长，影响系统性能和可伸缩性
源码 include/asm-frv/linux/smp\_lock.h

## 7.2 同步方案

1. 原则操作
- 原子操作对整数操作
2. 自旋锁
- 临界区小，使用自旋锁
3. 信号量
- 临界区大，使用信号量

# 8. per-CPU计数器
源码地址： include/linux/percpu\_counter.h
内核借助 struct percpu\_counter 数据结构实现计数器
- 在SMP或者NUMA等多CPU架构的系统时，描述每个CPU私有数据
- 为了避免多个 CPU 对全局数据的竞争而导致的性能损失，percpu 直接为每个 CPU 生成一份独有的数据备份，
- 每个数据备份占用独立的内存，CPU 不应该修改不属于自己的这部分数据，这样就避免了多 CPU 对全局数据的竞争问题
- 计数器的准确值，存储在内存的地址上。准确值是后是一个数据，每个数组对应系统中的一个CPU
- 描述每个CPU私有数据时，提供的机制
数据结构解析
- raw\_spinlock\_t lock 用于在需要准确值时，保护计数器(自旋锁)
- s64 count 计数器准确值
- s32 \_\_percpu \*counters 数组对应特定CPU，缓存对计数器的操作
常见方法
- void percpu\_counter\_destory (struct percpu\_counter *fbc)
- void percpu\_counter\_set (struct percpu\_counter *fbc, s64 amount)
- 对计数器设定为特定的值
- void \_\_percpu\_counter\_add (struct percpu\_counter *fbc, s64 amount, s32 batch)
- 对计数器的值，进行增减操作
- s64 \_\_percpu\_counter\_sum (struct percpu\_counter *fbc)
- 计算计数器的准确值
- int \_\_percpu\_counter\_compare (struct percpu\_counter *fbc, s64 rhs, s32 batch)