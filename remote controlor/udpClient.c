/*
 * @Author: gangliang lianggang@scu.edu.cn
 * @Date: 2022-09-12 11:18:17
 * @LastEditors: gangliang lianggang@scu.edu.cn
 * @LastEditTime: 2022-09-13 16:07:50
 * @FilePath: /netpro/2nd class/udpClient.c
 * @Description: Remote Control Sytem client(UDP)
 * 
 * Copyright (c) 2022 by gangliang lianggang@scu.edu.cn, All Rights Reserved. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUFSIZE 2048

int usage(char *cmd)
{
    fprintf(stderr,"usage for %s\n:",cmd);
    fprintf(stderr,"%s ipaddr port\n",cmd);
}

int main(int argc,char** argv)
{
    int sockfd;
    struct sockaddr_in serv;
    char send_buf[BUFSIZE];
    char recv_buf[BUFSIZE];
    int len,ret;


    if (3!=argc)
    {
        usage(argv[0]);
        exit(-1);
    }

    // 初始化
    sockfd=-1;
    memset(&serv,0,sizeof(struct sockaddr));
    memset(send_buf,0,BUFSIZE);
    memset(recv_buf,0,BUFSIZE);


    // 建立套接字
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if (0>sockfd)
    {
        perror("create socket error\n");
        exit(-1);
    }

    // 设置服务器
    serv.sin_family=AF_INET;
    serv.sin_port=htons(atoi(argv[2]));
    serv.sin_addr.s_addr=inet_addr(argv[1]);
    

    while(1)
    {
       
        fprintf(stderr,"rms>>>");
        fgets(send_buf,BUFSIZE,stdin);
        ret=sendto(sockfd,send_buf,BUFSIZE,0,(struct sockaddr*)&serv,sizeof(struct sockaddr));
        if (0>ret)
        {
            perror("error in sending data\n");
            exit(-1);
        }
        send_buf[strlen(send_buf)-1]='\0';
        if(0==strcmp(send_buf,"quit"))
        {
            fprintf(stderr,"remote control terminate\n");
            break;
        }

        ret=recvfrom(sockfd,recv_buf,BUFSIZE,0,NULL,NULL);
        if (0>ret)
        {
            perror("error in recving data\n");
            exit(-1);
        }
        recv_buf[ret-1]='\0';

        fprintf(stderr,"%s\n",recv_buf);
    }
    

    // 通讯结束，关闭套接字
    close(sockfd);
    




}