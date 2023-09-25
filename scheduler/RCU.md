# RCU机制

# 1.RCU简介

## 1.1 RCU是什么
是Linux内核中重要的同步机制，Read-Cpoy-Update,指的是读-拷贝-更新。
读写锁的替代品(读者和写者可以并发执行)

## 1.2 RCU的作用
Linux内有原子操作，读写信号量等锁机制，设计新的RCU机制是为了满足，极端非对称读/写同步机制。
不能满足的场景是:
- 读写信号量，使用了原子操作指令，原子的访问内存，多CPU共享一个变量，会让变量一致性编的很糟糕, 性能会下降
- 读写信号量，只允许多个读者同时存在，不允许读者和写者同时存在。
RCU机制的目标
- 希望读者线程没有同步开销，不需要额外的锁，原子操作指令和内存屏障

## 1.3 RCU原理
RCU记录所有指向共享数据的指针使用者，当要修改共享数据时，先创建副本，在副本中修改。
所有读访问线程离开临界区后，指针指向新的修改后副本的指针，删除旧数据。
RCU ReadLock允许多个线程读一个链表，同时允许修改链表

### 1.3.1写者修改数据流程
1. 复制生成一个副本
2. 修改副本内容
3. 使用新对象替换旧对象
4. 写者复制和更新的时候，读者可以先读数据

### 1.3.2写者删除数据流程
1. 必须等待所有访问被删除对象的读者结束访问，才能销毁数据

宽限期
- 等待所有读者访问结束的时间(grace period)

## 1.4 RCU的优缺点
1. 优势
- 读者和写者不需要同步，可以并发执行。(RCU目标就是减少读者开销, 常用于读者性能要求高的场景)
- 读者开销小，不需要获取任何的锁。不需要执行原子指令或内存屏障
- 不存在死锁问题
- 不存在优先级反转问题
- 不存在内存泄漏风险
- 支持实时延迟操作
2. 缺点
- 写者同步开销很大
- 写者之间需要互斥处理
- 使用其他同步机制


## 1.5 RCU使用场景
背景CPU和存储器的速度逐渐扩大，CPU速度加快，锁机制不能满足需求
所有锁都有使用场景
1. spin lock(自旋锁)
- 不区分reader/writer
- 不适合读写强度不对成的场景
2. rw lock(读写锁)
- 倾向于读者
3. seq lock(顺序锁)
- 倾向于写者
4. RCU
- 只能保护动态分配的数据结构，并且是通过指针访问该数据结构
- 受RCU保护的临界区，不能Sleep(SRCU不是本文的内容)
- 读写不对称，对写者的性能没有特别的要求，对读者性能要求比较高
- reader端，对于新旧数据不敏感
- 需要频繁读取数据，修改数据并不多的场景(文件系统中，搜索定位目录)

## 1.6RCU内核操作
源码位置: /include/linux/reclist.h
RCU的保护对象
1. 保护一般指针指向的数据
2. 保护双链表
- 遍历、修改、删除链表元素，需要使用RCU变体，标准函数后加\_rcu后缀

- static inline void list\_add\_rcu (struct list\_head \* new, struct list\_head \* head)
- static inline void list\_add\_tail\_rcu(struct list\_head \* new, struct list\_head \* head)
- static inline void list\_del\_rcu(struct list\_head \* entry)
- static inline void list\_replace\_rcu(struct list\_head \*old, struct list\_head \*new)



