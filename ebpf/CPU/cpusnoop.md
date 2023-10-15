# 关于CPU的监控和调试

# 1.CPU性能检查流程
1. 保证CPU运行状态，系统中CPU利用率
2. 确保系统负载受限于CPU
- 所有CPU利用率高还是单个CPU利用率高
- 检查运行队列延迟
- 检查CPU使用限制，容器限制Cgroup(空闲系统上还受限于CPU)
3. 量化CPU使用量
- 按照进程、CPU模式、CPUID来分解
- 系统时间过高，按照进程和系统调用类型统计系统调用使用频率。
- perf/sysstat
4. 性能剖析器采集调用栈信息，CPU火焰图展示
5. 针对CPU使用率高，定制开发
- 某个系统调用耗CPU时长，他去干什么了。
6. 测量硬中断消耗，可能对定时器分析器不可见
7. PMC测量每时钟周期CPU指令执行量，观察CPU阻塞。分析缓存命中率，温控等


# 传统工具
- uptime 展示系统负载平均值和运行时间
- top CPU使用时间和系统层面CPU模式时间
- mpstat 按每个CPU展示每种CPU模式时间
- perf 定时采样/调用栈、时间统计、PMC、跟踪点、USDT、probe 
- Ftrace 事件跟踪

- git clone https://github.com/brendangregg/FlameGraph
- cd FlameGraph
- perf record -F 49 -ag --sleep 30
- perf record -F 99 -a -v
- perf script --header | ./stackcollapse-perf.pl | ./flamegraph.pl > flamel.svg

# 