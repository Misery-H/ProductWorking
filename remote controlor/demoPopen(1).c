/*
 * @Author: gangliang lianggang@scu.edu.cn
 * @Date: 2022-09-13 14:53:23
 * @LastEditors: gangliang lianggang@scu.edu.cn
 * @LastEditTime: 2022-09-13 14:54:05
 * @FilePath: /netpro/2nd class/demoPopen.c
 * @Description: demostrate the function popen to put the execution result for cmd into memory buffer
 * 
 * Copyright (c) 2022 by gangliang lianggang@scu.edu.cn, All Rights Reserved. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>





int main(void)
{
    FILE * fp;
    char buf[256];

    fp=NULL;
    memset(buf,0,256);

// r 表示连接到命名的输入，w表示连接到命令的输入，所谓读写是针对当前主控进程
    if ((fp = popen("ls -l", "r")) == NULL)
    {
        perror("open failed!");
        return -1;
    }

    // 每次直接从管道中读取最大容量的内容，如果不为空，直接打印结果
    while (fgets(buf, 255, fp) != NULL)
    {
        
        printf("%s", buf);
    }

    if(pclose(fp) == -1)
    {
        perror("close failed!");
        return -2;
    }
    return 0;
}
