# 性能分析

## 1.分析问腿

### 1.1分析设计

- 关注目的：我们面对一个什么样的性能问题，以此为核心分析
- 延迟: 多久可以完成一次请求和操作
- 速率: 每秒操作或请求的速度
- 吞吐量: 每秒传输的数据量
- 利用率: 资源在某段时间内的繁忙程度
- 成本: 开销，性能的比例

- CPU 运行代码的事件
- 等待IO 网络 锁的时间
- 等待CPU调度的时间
- 不建议使用大而全的工具是因为多个事件会带来显著的运行开销
- 小组件，研究特定的时间开销和运行开销

### 1.2分析工作

- 原型软件或硬件的性能特征分析
- 开发阶段，集成阶段之前的性能分析
- 发布前后非回归测试
- 基准测试，宣发数据支持
- 概念验证测试
- 监控生产环境中的软件
- 故障排查时的性能分析

## 2.分析方法论

### 2.1 业务负载画像

- 负载是谁产生的(进程ID\用户ID\进程名\IP地址)
- 负载为什么会产生(代码路径\调用栈\火焰图)
- 负载的组成是什么(IOPS\吞吐\负载类型)
- 负载如何随时间变化(周期摘要比较)

### 2.2下钻分析/清单分析

- 从指标开始，拆分指标为多个组成部分，再拆分最大的组件，直到定位到问题根因
- 从业务高层级开始分析
- 检查下一层级业务细节
- 检查线索

- 请求延迟时100ms
- 10ms CPU 90ms 脱离CPU
- 脱离CPU中 89ms阻塞在文件系统上
- 3ms阻塞在锁上, 86m阻塞在存储设备上
- 78ms在写操作，11ms在读操作
- 77ms在时间戳更新

### 2.3USE方法论

- 使用率
- 饱和度
- 错误

## 4.60s问题定位

- uptime 看平均负载
- dmesg | tail 看OOM和TCP丢包，SNMP计数器
- vmstat 1  查看虚拟内存统计工具
- mpstat -P ALL 1 打印每个CPU状态
- pidstat 1 展示进程CPU的使用情况
- iostat -xz 1 显示存储设备的IO指标，
- free -m 显示可用内存
- sar -n DEV 1 查看网络设备吞吐
- sar -n TCP, ETCP 1 查看TCP链接数量和重传
- top 综合性查看资源情况

## 5.BCC工具快速定位问题