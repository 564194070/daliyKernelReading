# bpftrace 例子

# 1.常见实例
## 统计函数调用次数
- bpftrace -e 'kprobe:vfs\_read {@[comm] = count();}'

## 统计vfs\_read返回值
- bpftrace -e 'kretprobe:vfs\_read {@bytes = hist(retval);}'
- 读写字节数或错误码

# 2.bpftrace使用

## 2.1事件源

- kprobe 动态插装
- uprobe 动态插装
- tracepoint 静态跟踪
- software 静态跟踪
- usdt    静态跟踪
- profile 定期事件采集
- interval 周期事件
- hardware PMC事件
- BEGIN/END 合成事件

## 2.2动作

1. 过滤
- 谓词条件
2. 事件输出
- printf()
3. 基础变量
- global $local per[tid]
4. 内置变量
- pid tid comm nsecs
5. 关联数组
- key[value]
6. 频率计数
- count ++
7. 统计值
- min max sum avg stats
8. 直方图
- hist lhist
9. 时间戳/时间差
- nsecs
10. 调用栈
- kstack ustack
11. 符号解析
- ksym() kaddr() usym() uaddr()
12. 访问C结构体
- ->
13. 数组访问
- []
14. shell命令
- system()
15. 打印文件
- cat()
16. 基于位置的参数
- $1 $2

## 3.常用工具
1. CPU
2. 内存
3. 文件系统
4. 存储IO
5. 网络
6. 安全
7. 编程语言
8. 应用相关
9. 内核
10. 容器
11. 虚拟机
12. 调试器

## 4.bpftrace 使用方法

1. bpftrace -e (fileName)
- 启动EBPF程序

2. probes {actions}
probes /filter/ {actions}

3. type:identifier1[:identifier2[...]]
- 

4. 更多通配符
- kprobe:vfs_*

5. 过滤器
- /pid == 123/

6. 动作
- {action one; action two; action three}

7. 变量
- 进程id， pid
- 进程名称comm
- 时间戳nsecs
- curtask 当前task\_struct
- $临时变量
- @映射表变量
- @path[pid,$fd]=str(arg0) 符合Key

8. 控制流
- == /!=/>/</>=/<=/&&/||/
- bool? true :false
- if (test) {true} else {false}
- unroll (count) {statements}

9. 内置变量
- pid/tid/uid/username
- nsecs/elapsed/cpu/comm
- kstack/ustack
- arg0/argv/args/
- retval
- func
- probe
- curtask
- cgroup

10. 内置函数
- printf
- time
- join 打印字符串数组，空格分隔
- str 从指针返回字符串
- kstack/ustack 返回内核调用栈
- ksym/usym 分析地址，返回函数名
- kaddr/uaddr 名字翻译成地址
- reg 返回值送到寄存器
- ntop 返回字符串表示的IP地址
- system 执行shell命令
- cat 打印文件内容
- exit()

11. 映射表操作函数
- count() 统计次数
- sum()
- avg()
- min()
- max()
- stats 返回事件次数，平均值总和
- hist 打印2的幂次方直方图
- lhist 打印线性机房图
- delete(@m[key]) 删除键值对
- clear 清空映射表
- zero 设置映射表为0