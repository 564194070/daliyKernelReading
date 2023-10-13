# bpftrace 例子


## 统计函数调用次数
bpftrace -e 'kprobe:vfs\_read {@[comm] = count();}'