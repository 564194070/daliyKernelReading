# 1.物理地址信息

## 1.1 物理地址总线
- 处理器在系统总线上看到的地址
- 使用RISC精简指令集的处理器，只实现一个物理地址空间，外围设备和物理内存使用统一的物理地址空间
- 有些处理器架构把分配给外围设备的物理区域称为设备内存
- 处理器通过外围设备控制器的寄存器访问外围设备，寄存器分为控制寄存器，状态寄存器，数据寄存器三种
- 外围设备的寄存器，通常被连续编址。
- 处理器对外围设备寄存编址方式分为两种，IO映射(I/O-mapper)和内存映射(memory-mapped)

## 1.2 IO-Mapper和Memory-mapper

1. IO地址空间/IO端口空间

- CPU通过IO指令，in/out指令，访问空间中的地址单元

2. Memory

- 像访问内存单元一样，访问外围设备，不需要提供专门的IO指令 把外围寄存器地址映射到虚拟内存空间
- 应用程序只能通过虚拟地址空间，访问外设的寄存器
- 内核提供API，把外设寄存器的物理地址，映射到虚拟地址空间
- ARM64架构，有两种内存类型: 
- 1.正常内存(Normal Memory):
- 物理内存+只读存储器(ROM) 共享属性，外部共享DMA，内部共享多核心，处理器缓存
- 2.设备内存(Device Memory):
- 分配给外围设备寄存器，相关的物理地址区域 共享属性，外部共享，处理器不能缓存，

# 2.内存映射原理

将数据映射到进程虚拟内存地址空间
进程读写操作这段内存，系统自动回写页面到文件磁盘
1. 两个进程可以使用共享的文件映射实现共享内存,
2. 匿名映射通常是私有映射，共享的匿名映射只可能出现在父子进程之间
3. 进程虚拟地址空间中，代码段和数据段是私有文件映射，未初始化段、堆栈是私有匿名映射。

## 2.1 内存映射

- 创建内存映射时，在进程的用户虚拟地址空间中分配一个虚拟内存区域，内核采用延迟分配的策略，进程第一次访问虚拟页，产生缺页异常

1. 文件映射，分配物理页，把文件指定区间数据读到物理页中，然后页表中把虚拟页映射到物理页

- 文件支持内存映射，把文件的一个区域映射到进程的虚拟地址空间，数据源是存储设备上的文件

2. 匿名映射，分配物理页，然后再页表中把虚拟页映射到物理页
- 没有文件支持的内存映射，把物理内存映射到进程的虚拟地址空间，没有数据源


## 2.2 线程启动映射过程
在虚拟地址空间（mm_struct）创建一个虚拟映射区域（vm_area_struct）
mmap在进程当前的虚拟地址空间当中，空闲连续虚拟地址空间作为内存虚拟映射地址区域，初始化并插入进程虚拟地址区域的链表
内核创建文件物理地址和内存虚拟地址之间的映射关系
进程读写访问操作虚拟地址，对堆发起访问。查询页表，发现内容不在内存这块物理页面上(映射关系有了，但是内容没移动)，
换页过程
查询页表发现内存不在内存，先找swap，没有就nopage，把缺页从磁盘调入内存，进行读写操作，一段时间后系统回写脏页到磁盘
msync 强制同步写入文件
# 3.数据结构
# 4.系统调用