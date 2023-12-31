# BPF简介

## 1.BPF

- 背景:2014年进入内核主线，提供了在各种内核事件和应用程序发生时，执行一小段程序的机制
- 组成部分: 解释器+BPF指令动态转化为本地指令的即时编译器(JIT)
- 执行过程: BPF指令验证器(verifer)，确保不会自身崩溃和损坏内核
- 主要方向: 网络、可观测性、安全
- 只在存活期间被激活

### 1.1 跟踪、嗅探、采样、剖析、可观测性

1. 嗅探(snoop) = 跟踪(trace) = 事件记录
2. 采样(sampling) = 性能剖析样本 = profiling 通过数据来描述目标的大致规律(基于计时器完成采样)

- 优点 性能开销比跟踪小，因为对事件只做一部分测量
- 缺点 只会提供大致画像，会遗漏事件

3. 可观测性(observability) = 通过全面观测来理解系统的运行

- 跟踪、采样、计数器工具都属于
- 基准测量不属于，他会改变业务负载状态

4. BPF是一种可编程型跟踪分析工具

### 1.2不同类型的工具

1. BCC

- BPF编译器集合，BPF Compiler Collection
- 提供编写内核BPF程序的环境，是libbcc和libbpf的前身
- 编写复杂脚本和后台进程

2. Bpftrace

- 创建BPF工具高级语言支持
- 编写单行程序

3.Ply

- 轻量级，更适合嵌入式

4. IO Visor

- BCC和bpftrace所属的Linux基金会

## 1.3技术方案

1. kprobe&&uprobe

- 动态插桩技术，在生产环境对正在运行的软件插入观测点，在函数开始或结束的位置插桩
- 原理是给程序任意指令地址插入断点

2. tracepoint&&USDT

- 版本更新，函数重命名和移除，接口稳定性低
- 会增加开发者的维护成本
- 建议现场时静态跟踪，再尝试动态跟踪
- CONFIG_FTRACE_SYSCALLS 管理系统调用跟踪点