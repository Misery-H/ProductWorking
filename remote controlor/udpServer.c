/*
 * @Author: gangliang lianggang@scu.edu.cn
 * @Date: 2022-09-13 15:28:41
 * @LastEditors: gangliang lianggang@scu.edu.cn
 * @LastEditTime: 2022-09-13 16:16:34
 * @FilePath: /netpro/2nd class/udpServer.c
 * @Description: Remote Ctrl System Server(udp server)
 * 
 * Copyright (c) 2022 by gangliang lianggang@scu.edu.cn, All Rights Reserved. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>


#define PORT 8900
#define BUFSIZE 2048

/**
 * @description: 将客户端内容作为命令执行，并发送给客户端
 * @param:cmd:待处理的命令
 *       :sockfd: 通讯的套接字
 *       :client:对应客户端地址
 * @return {*}：0 正确，-1 失败
 */

int execute(char* cmd,int sockfd,struct sockaddr_in client)
{
    char buf[BUFSIZE];
    FILE* fp;
    char* errbuf="command cannot execute\n";
    int ret;
    int counter;

    fp=NULL;
    counter=0;
    memset(buf,0,BUFSIZE);

    if ((fp = popen(cmd, "r")) == NULL)
    {
        perror("open failed!");
        ret=sendto(sockfd,errbuf,strlen(errbuf),0,(struct sockaddr*)&client,sizeof(struct sockaddr));
        return -1;
    }

    while((!feof(fp))&&(counter<2048))
    {
        buf[counter]=fgetc(fp);
        counter++;     
    }
    ret=sendto(sockfd,buf,strlen(buf),0,(struct sockaddr*)&client,sizeof(struct sockaddr));
    if(0>ret)
        return -1;

    return 0;

}

int main(int argc,char** argv)
{
    // declare variable
    int sockfd;
    struct sockaddr_in serv, client;
    int opt;
    int ret;
    int len;
    char send_buf[BUFSIZE],recv_buf[BUFSIZE];

    // variable initialization
    sockfd=-1;
    opt=SO_REUSEADDR;
    ret=-1;
    len=-1;
    bzero(&serv,sizeof(struct sockaddr));
    bzero(&client,sizeof(struct sockaddr));
    bzero(send_buf,BUFSIZE);
    bzero(recv_buf,BUFSIZE);

    // create socket
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(0>sockfd)
    {
        perror("error in creating socket]n");
        exit(-1);
    }

    // set the socket
    // allow re-bind
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int));

    //set the value for serv
    serv.sin_family=AF_INET;
    serv.sin_addr.s_addr=INADDR_ANY;
    serv.sin_port=htons(PORT);

    // bind the socket serv
    ret=bind(sockfd,(struct sockaddr*)&serv,sizeof(struct sockaddr));
    if(0>ret)
    {
        perror("error in binding\n");
        exit(-1);
    }

    // 开始通讯：所有信息当成命令
    // 交给函数execute处理，并发送信息给对应客户端
    len=sizeof(struct sockaddr);
    while(1)
    {
        memset(recv_buf,0,BUFSIZE);        
        ret=recvfrom(sockfd,recv_buf,BUFSIZE,0,(struct sockaddr*)&client,&len);
        if(0>ret)
        {
            perror("error in recving data\n");
            continue;
        }
        recv_buf[ret-1]='\0';
        // printf("%s\n##\t%lu\t\n",recv_buf,strlen(recv_buf));
        // int i;
        // for(i =0;i<strlen(recv_buf);i++){
        //     printf("%c\n",recv_buf[i]);
        // }
        if (0==strcmp(recv_buf,"quit\n"))
        {
            fprintf(stderr,"server is terminate by client\n");
            break;
        }
        
        execute(recv_buf,sockfd,client);

    }
    fprintf(stderr,"server is down\n");
    close(sockfd);
    
}
