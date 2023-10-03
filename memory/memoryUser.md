# 用户空间内存

##  1.常见内存使用方法
第一层
- malloc/free new/delete
第二层
- ptmalloc(glibc)
- jemalloc(Freebsd)
- tcmalloc(google)
第三层
- 系统调用