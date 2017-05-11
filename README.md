# ICS-Introduction-to-Computer-Systems
ICS: Introduction to Computer Systems
## ������� 
``` bash
gcc -g -O -c main.c  ���� .o �ļ�
gcc -g -O2 -o p main.c  ���ɿ�ִ���ļ�
objdump -r -d main.o ������ .o �ļ�
objdump -x -d proj ������ ��ִ���ļ� 
```
## �γ���Դ 
```
��������� http://csapp.cs.cmu.edu/

�Ϻ���ͨ��ѧ�ÿι����� http://ipads.se.sjtu.edu.cn/courses/ics/schedule.shtml
cms �γ̹����� http://www.cs.cmu.edu/~./213/schedule.html
��ʢ�ٴ�ѧ�� https://courses.cs.washington.edu/courses/cse351/17sp/index.html ����Ӳ���ӿڣ�

cmu ѧ���ʼ� �� http://wdxtub.com/2016/04/16/thin-csapp-0/
```
1987728
���Ϻ������ PPT�� ��  CMU �� ʵ��



## Labs
- ʵ��7    Memory Allocator 
- ʵ��8   Cache
- ʵ��9   Tiny Shell
- ʵ��10 Proxy

## �½� 
``` text 
�� �� ���� ��ϵͳ�����г���
�� 7 �� ����
7.1  ��������������
7.2  ��̬����
7.3 Ŀ���ļ�
7.4 ���ض�λĿ���ļ�
7.5 ���źͷ��ű�
7.6 ���Ž���
  7.6.1 ��������ν������ض���
  7.6.2 �뾲̬������
  7.6.3 ���������ʹ�þ�̬������������
7.7 �ض�λ
   7.7.1 �ض�λ��Ŀ
   7.7.2 �ض�λ��������
7.8 ��ִ��Ŀ���ļ�
7.9 ���ؿ�ִ��Ŀ���ļ�
7.10 ��̬���ӹ����
7.11 ��Ӧ�ó����м��غ����ӹ����
7.12 ��λ���޹صĴ��루PIC��
7.13 ����Ŀ���ļ��Ĺ���

�� 8 �� �쳣������
8.1 �쳣
  8.1.1 �쳣����
  8.1.2 �쳣�����
  8.1.3 Linux / IA32 ϵͳ���쳣
8.2 ����
  8.2.1 �߼�������
  8.2.2 ������
  8.2.3 ˽�е�ַ�ռ�
  8.2.4 �û�ģʽ���ں�ģʽ
  8.2.5 �������л�
8.3 ϵͳ���ô�����
8.4 ���̿���
  8.4.1 ��ȡ���� ID
  8.4.2 ��������ֹ����
  8.4.3 �����ӽ���
  8.4.4 �ý�������
  8.4.5 ���ز����г���
  8.4.6 ���� fork �� execve ���г���
8.5 �ź�
  8.5.1 �ź�����
  8.5.2 �����ź�
  8.5.3 �����ź�
  8.5.4 �źŴ�������
  8.5.5 ����ֲ���źŴ���
  8.5.6 ��ʾ��������ȡ�������ź�
  8.5.7 ͬ�����Ա�������Ĳ�������
8.6 �Ǳ�����ת
8.7 �������̵Ĺ���

�ʼǣ� 
������ܣ� ��������ʹ��ȫ�ֱ����� ���һ��Ҫ�ã�
1 ʹ�þ�̬����
2 ����ȫ�ֱ�����ʱ���ʼ��
3 ע��ʹ�� extern �ؼ���

��ʮ���� ������
12.4 �׽��ֽӿ�


12. 5 Web ������
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

Problem III (*������)
0xd  bar  R_386_ PC32
xx    foo1   R_386_ PC32
xx    foo2    R_386_ PC32
xx    foop2  R_386_ PC32

xx    x          R_386_ 32
xx    xp        R_386_ 32


Problem 4 ��*�����⣩
1 1115  +1s 
