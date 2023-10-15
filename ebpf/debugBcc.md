# BCC 调试工具

## 1.BCC调试工具列表

### 1.1程序调试工具

1. ./file.py --ebpf
- BPF C
2. BPF(debug=DEBUG_PREPROCESSOR)
- BPF C
3. BPF(debuf=DEBUG_LLVM_IR)
- LLVM中间表现形式
4. BPF(debuf=DEBUF_BPF)
- BPF字节码
5. bpftool prog dump xlated
- BPF字节码
6. bpftool prog dump jitted
- 机器码

- BPF(text=bpf\_text, debug=0x2)
- 1 打印编译好的LLVM中间表现形式
- 2 在分支处打印BPF字节码和寄存器状态
- 4 打印预处理结果
- 8 打印原代码内嵌汇编
- 10 打印所有指令寄存器状态
- 20 打印BTF调试信息

### 1.2状态调试工具

1. bpflist
- bcc 进程
2. bpftool map show
- 映射表
3. bpftool prog show
- 已挂在程序
4. bpflist -vv
- kprobe/uprobe

### 1.3打印缓冲区
- bpftool prog tracelog

### 1.4清理工具
- reset-trace.sh