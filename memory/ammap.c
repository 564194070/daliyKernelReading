#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


// 两个进程共享内存通讯

typedef struct 
{
    char name[6];
    int age;
}people;

int main(int argc, char **argv)
{
    int fd,  i;
    people *p_map;
    char temp;

    fd = open(argv[1], O_CREATE|O_RDWR|O_TRUNC,00777);
    lseek(fd, sizeof(people)*5-1, SEEK_SET);
    write(fd, "",1);
    
    p_map = (people*)mmap(NULL, sizeof(people)*10, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    // NULL start 映射区的开始地址， 映射区长度， 内存保护标志允许读写，指定映射对象关系(对共享区写入)，文件描述符，被映射对象的起点0)
    if (p_map == (void*)-1)
    {
        fprintf(stderr, "mmap :%s\n", strerror(errno));
        return ;
    }
    close(fd);

    for (size_t i = 0; i < 10; i++)
    {
        /* code */
        (*(p_map + i)).name[1] = '\0';
        memcpy((*(p_map + i)).name, &temp, 1);
        (*(p_map + i)).age = 30 + i;
    }
    
    sleep(10);
    munmap(p_map, sizeof(people)*10);

    return 0;
}