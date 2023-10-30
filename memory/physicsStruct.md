# 内存物理组织结构

## 1.系统调用sys_mmap
- 源码地址mm/mmap.c 主要工作给了do\_mmap完成 创建内存映射

- 检查偏移是不是页的整数倍(错误返回-EINVAL)
- sys_mmap_pgoff

- 文件映射 (根据文件描述符在进程打开的文件列表中找file实例)
- 匿名巨形页映射(在hugetlbfs文件系统创建文件，anon_hugepage)
- vm\_mmap\_pgoff 以写者身份申请读写信号量。

- do\_mmap 把创建内存映射的主要工作委托给此函数

- get\_unmapped\_area(根据特殊情况分配虚拟地址范围)
- 计算虚拟内存标识
- mmap\_region(创建虚拟区域) may_expand_vm 检查进程虚拟内存限制；是否和旧的虚拟内存与去重叠do_unmanmap删除；是否有私有可写映射，是否有可以和现有虚拟内存区域合并vma_merge
- 创建新的虚拟内存区域
- 文件映射 call_mmap
- 共享匿名映射 shmem_zero_setup
- vma_link
- vma_set_page_prot

- 释放读写信号量
- 调用者要求页锁定在内存，uo填充页表，允许阻塞，调用函数
- 分配物理页，并在页表把虚拟页映射到物理页

## 2.系统调用sys_munmap
- 源码位置mm/mmap.c 删除内存映射 参数 起始地址和长度

- vm_munmap
- do_munmap
- vma = find_vma 根据起始位置，找到删除元素的位置
- error = __split_vma 分裂内存，只删除需要处理的部分
- munlock_vma_pages_all 解除锁定(虚拟内存在锁在内存中，不允许换出)
- detach_vmas_to_be_unmapped 把所有删除目标从进程虚拟内存区域链表和数中删除，单独组成临时链表
- unmap_region 在进程的页表中，删除映射。从处理器，页表缓存删除映射
- arch_unmap 处理器架构相关处理
- remove_vma_list 调用此函数删除所有目标


## 3.物理内存组织结构
- 非一致内存访问 NUMA 内存被划分多个内存结点的多处理器系统，访问内存的时间，取决于处理器和内存的距离
- 对称多处理器 SMP 一致内存访问，所有处理器访问内存花费的时间是相同的