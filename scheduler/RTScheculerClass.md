# 实时调度类

## 1.1 实时调度实体 struct sched\_rt\_entity  
表示一个实时调度的实体
源码:include/linux/sched.h

- struct rt\_entity 
- struct list\_head run\_list 运行列表
- unsigned long timeout 判断当前进程时间是否超过RLIMIT\_RTTIMR(实时时间)
- unsigned int time\_slice RR轮询策略的调度时隙

- struct sched\_rt\_entity \*back 作为dequeue\_rt\_stack中作为临时变量
- struct sched\_rt\_entity \*parent 指向上一层的调度实体
- struct rt\_rq  \*rt\_rq 当前调度实体所在就绪队列
- struct rt\_rq \*my\_q 当前调度实体的子调度实体所在就绪队列


## 1.2 实时调度类 struct sched\_class rt\_sched\_class 
源码:kernel/sched/rt.c

- .next = fair\_sched\_class 
- enqueue\_task\_rt   将一个task放入就绪队列的头部或尾部
- dequeue\_task\_rt   将一个task从就绪队列删除 
- yield\_task\_rt         主动放弃执行
- pick\_next\_task\_rt    核心调度器，选择就绪队列那个任务被调度
- put\_prev\_task\_rt     当一个任务将要被调度出时执行


- select\_task\_rq\_rt    核心调度器给任务特定CPU，用于将任务分发到不同的CPU上执行
- set\_curr\_task\_rt     当任务修改其调度类或修改其他任务组时，将调用这个函数
- task\_tick\_rt	  当时钟中断触发时被调用，主要更新新进程运行统计信息，及是否需要调度


## 1.3 实时调度类核心操作
源码:kernel/sched/rt.c

1. 进程插入 enqueue\_task\_rt
- 更新调度信息，将调度实体插入到对应优先级队列的末尾
- static void enqueue\_task\_rt (struct rq \*rq, struct task\_struct \*p, int flags)
- enqueue\_rt\_entity 实际负责进程插入的函数。将当前调度实体，添加到对应优先级调度链表头尾上，取决于flag判断头尾(ENQUEUE\_HEAD)
- enqueue\_pushable\_task 添加到对应HASH表中

2. 选择插入 enqueue\_rt\_entity
- static void enqueue\_rt\_entity (struct sched\_rt\_entity \*rt\_se, bool head)

3. 进程选择 pick\_next\_task\_rt
- 实时调度器会选择优先级最高的实时进程来运行
- static struct task\_struct \* \_pick\_next\_task\_rt (struct rq \*rq)
- do while 遍历组调度中的每个进程 执行进程实体
- p\-\> se.exec\_start = rq\_clock\_task(rq); 更新执行域
 
4. 进程选择 \*pick\_next\_rt\_entity 
- static struct sched\_rt\_entity \*pick\_next\_rt\_entity (struct rq \*rq, struct rt\_rq \*rt\_rq)
- idx = sched\_find\_first\_bit(array-> bitmap) 找到一个可用的实体
- queue = array-> queue + idx 从链表组中找出对应的实体链表节点
- next = list\_entry (queue-> next, struct sched\_rt\_entity, run\_list) 返回找到的运行实体

5. 进程删除 dequeue\_task\_rt 
- 从优先级队列中删除实时进程，更新调度信息，将进程添加到队尾
- static void dequeue\_task\_rt (struct rq \*rq , struct task\_struct \*p, int flags)
- update\_curr\_rt(rq) 更新调度信息
- dequeue\_rt\_entity(rt\_se)
- dequeue\_pushable\_task(rq, p)
