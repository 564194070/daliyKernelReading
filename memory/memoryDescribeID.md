# 内核描述虚拟内存空间的数据结构

# 1. mm\_struct 内存描述符
使用mm\_struct描述进程的用户虚拟地址空间
进程空间所有的信息
源码：include//linux/mm\_types.h

1. struct vm\_area\_struct \*mmap
- 虚拟内存区域的链表
2. struct rb\_root mm\_rb 
- 虚拟内存区域红黑树
3. get\_unmapped\_area 
- 在内存映射区域找到一个没有映射的区间
4. mmap\_base
- 内存映射区域起始地址
5. task\_size
- 用户虚拟地址空间的长度
6. pgd
- 指向页全局目录，指向第一级页表
7. mm\_users(atomic)
- 共享一个用户虚拟地址空间的进程的数量，线程组包含进程的数量
8. mm\_count
- 内存描述符的引用计数
9. mm\_context\_t context
- 处理器架构特定内存管理上下文
10. start\_code end\_code
- 代码段的起始地址和结束地址
11. start\_data end\_data
- 数据段的起始地址和结束地址
12. start\_brk brk
- 堆的起始结束地址
13. stack\_start
- 栈的起始地址
14. arg\_start arg\_end env\_start env\_end
- 参数和环境便变量的起始地址结束地址
15. hiwater\_rss 
- 进程所拥有的最大叶框数
16. hiwater\_vm 
- 进程线性区中最大页数
17. total\_vm
- 进程地址空间大小(页数)
18. locked\_vm
- 锁住不能换出页个数
19. stack\_vm
用户堆栈中的页数

# 2.进程和内存描述符的对应
1. 进程指向内存描述符,内存描述符指向虚拟页
- task\_struct -> mm\_struct -> page\_table -> 物理内存
- task\_struct 
2. 进程的虚拟地址空间由两个数据结构描述，
- mm\_struct(至高层次): 描述整个用户虚拟地址空间
- vm\_area\_struct(较高层次): 描述虚拟地址空间的一个区间(虚拟区)