# 实时调度类



# 1.1 实时调度实体 struct sched\_rt\_entity  
表示一个实时调度的实体
源码:include/linux/sched.h

- struct list\_head run\_list 运行列表
- unsigned long timeout 判断当前进程时间是否超过RLIMIT\_RTTIMR(实时时间)
- unsigned int time\_slice RR轮询策略的调度时隙

- struct sched\_rt\_entity \*back 作为dequeue\_rt\_stack中作为临时变量
- struct sched\_rt\_entity \*parent 指向上一层的调度实体
- struct rt\_rq  \*rt\_rq 当前调度实体所在就绪队列
- struct rt\_rq \*my\_q 当前调度实体的子调度实体所在就绪队列


# 1.2 实时调度类 struct sched\_class rt\_sched\_class 

源码:kernel/sched/rt.c

- &fair\_sched\_class 
- enqueue\_task\_rt   将一个task放入就绪队列的头部或尾部
- dequeue\_task\_rt   将一个task从就绪队列删除 
- yield\_task\_rt         主动放弃执行
- pick\_next\_task\_rt    核心调度器，选择就绪队列那个任务被调度
- put\_prev\_task\_rt     当一个任务将要被调度出时执行


- select\_task\_rq\_rt    核心调度器给任务特定CPU，用于将任务分发到不同的CPU上执行
- set\_curr\_task\_rt     当任务修改其调度类或修改其他任务组时，将调用这个函数
- task\_tick\_rt	  当时钟中断触发时被调用，主要更新新进程运行统计信息及是否需要调度


-
-
