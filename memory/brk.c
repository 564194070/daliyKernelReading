#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// 预设开辟长度
#define MAX 1024

int main()
{
    // 观察当前栈空间地址
    int *p = sbrk(0);
    int *old = p;

    // 开辟新的堆栈空间
    p = sbrk(MAX * MAX);
    // 判断是否开辟成功
    if (p == (void*)(-1))
    {
        perror("sbrk error \n");
    }


    // 销毁开辟的新内存
    sbrk(-MAX * MAX);
    printf ("pid = %d\n", getpid());
    return 0;
}

/*
    如何查看进程内存使用
    cat /proc/pid/maps
    1. 本段在虚拟内存中的地址范围 
    2. 本段的权限
    3. 偏移地址，映射地址在文件的偏移
    4. 主设备号和次设备号
    5. 文件索引节点号
    6. 映射文件名
*/