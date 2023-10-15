# 1.常见工具

## 1.1execsnoop

- 监控系统调用: exec\execve
- 作用: 跟踪每个新创建的进程
- 优点: 可以监控到短时进程
- 能力: 业务负载画像(给当前业务负载定性)、微基准测试

## 1.2 biolatency

- 监控系统调用: 
- 作用: 块设备I/O(disk I/O)延迟直方图，显示IO事件的响应时间。
- 优点: 可以发现关于延迟离群点

## 1.3 opensnoop

- 监控系统调用 open/openat
- 作用: 监控触发打开文件系统的进程和行为
- 优点: 可以监控频次问题、只显示打开失败行为-x、以及文件保护

## 1.4 ext4slower

- 跟踪ext4文件系统常见的操作，读写打开同步
- 把耗时超过某个阈值的操作打印出来

## 1.5 biosnoop

- 将每一次磁盘IO请求打印出来，包含延迟之类细节信息

## 1.6cachestat

- 打印一行摘要，展示文件系统缓存信息
- 检测缓存命中率低的问题

## 1.7tcpconnect

- 每次主动TCP建立连接connect时，打印一行信息

## 1.8tcpaccept

- 每次被动TCP建立连接时，tcpaccept打印一行信息

## 1.9tcpretrans

- 每次TCP重传数据包，会打印一条记录
- 通过STATUS判断网络问题

## 1.10runqlat

- 对线程等待CPU运行时间统计
- 定位超出预期的CPU等待时间

## 1.11profile

- CPU剖析器，什么代码消耗了CPU资源
- 周期性采样，然后打印

## 1.12funccount

- 对函数调用进行计数

## 1.13stackcount

- 对引发某事件的函数栈进行计数
- 火焰图
- stackcount -f -P -D 10 ktime_get > out.count.txt
- git clone http://github.com/brendangregg/FlameGraph
- cd FlameGraph
- ./flamegraph.pl --hash --bgcolors=grey < ./out.count.txt > test.svg

## 1.14trace

- 定制化打印事件细节信息
- trace 'do_sys_open "%s", arg2'
- 显示第X个参数

## 1.15argdist

- 统计事件参数分布