# ICS-Introduction-to-Computer-Systems
ICS: Introduction to Computer Systems
## 编译命令： 
``` bash
gcc -g -O -c main.c  生成 .o 文件
gcc -g -O2 -o p main.c  生成可执行文件
objdump -r -d main.o 反编译 .o 文件
objdump -x -d proj 反编译 可执行文件 
```
## 课程资源 
```
本书官网： http://csapp.cs.cmu.edu/

上海交通大学该课官网： http://ipads.se.sjtu.edu.cn/courses/ics/schedule.shtml
cms 课程官网： http://www.cs.cmu.edu/~./213/schedule.html
华盛顿大学： https://courses.cs.washington.edu/courses/cse351/17sp/index.html （软硬件接口）

cmu 学生笔记 ： http://wdxtub.com/2016/04/16/thin-csapp-0/
```
1987728
用上海交大的 PPT， 用  CMU 的 实验



## Labs
- 实验7    Memory Allocator 
- 实验8   Cache
- 实验9   Tiny Shell
- 实验10 Proxy

## 章节 
``` text 
第 二 部分 在系统上运行程序
第 7 章 链接
7.1  编译器驱动程序
7.2  静态链接
7.3 目标文件
7.4 可重定位目标文件
7.5 符号和符号表
7.6 符号解析
  7.6.1 链接器如何解析多重定义
  7.6.2 与静态库链接
  7.6.3 链接器如何使用静态库来解析引用
7.7 重定位
   7.7.1 重定位条目
   7.7.2 重定位符号引用
7.8 可执行目标文件
7.9 加载可执行目标文件
7.10 动态链接共享库
7.11 从应用程序中加载和链接共享库
7.12 与位置无关的代码（PIC）
7.13 处理目标文件的工具

第 8 章 异常控制流
8.1 异常
  8.1.1 异常控制
  8.1.2 异常的类别
  8.1.3 Linux / IA32 系统的异常
8.2 进程
  8.2.1 逻辑控制流
  8.2.2 并发流
  8.2.3 私有地址空间
  8.2.4 用户模式和内核模式
  8.2.5 上下午切换
8.3 系统调用错误处理
8.4 进程控制
  8.4.1 获取进程 ID
  8.4.2 创建和终止进程
  8.4.3 回收子进程
  8.4.4 让进程休眠
  8.4.5 加载并运行程序
  8.4.6 利用 fork 和 execve 运行程序
8.5 信号
  8.5.1 信号术语
  8.5.2 发送信号
  8.5.3 接收信号
  8.5.4 信号处理问题
  8.5.5 可移植的信号处理
  8.5.6 显示地阻塞和取消阻塞信号
  8.5.7 同步流以避免讨厌的并发控制
8.6 非本地跳转
8.7 操作进程的工具

笔记： 
如果可能， 尽量避免使用全局变量， 如果一定要用：
1 使用静态变量
2 定义全局变量的时候初始化
3 注意使用 extern 关键字

第十二章 网络编程
12.4 套接字接口


12. 5 Web 服务器
```

## Homework
Homework 1 
Problem 1
ch .data
num .bss
foo .text
i,(.data) j      stack 

Problem II
global
referenced global 
referenced global
local 
global
global 
local

Problem III (*有问题)
0xd  bar  R_386_ PC32
xx    foo1   R_386_ PC32
xx    foo2    R_386_ PC32
xx    foop2  R_386_ PC32

xx    x          R_386_ 32
xx    xp        R_386_ 32


Problem 4 （*有问题）
1 1115  +1s 
