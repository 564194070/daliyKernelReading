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
- 去判断页面对齐后是否超出边界
2. 
3. 

