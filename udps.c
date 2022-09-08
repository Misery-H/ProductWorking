/**********************************************************\
*                                                         *\
*  program name:tcpserver.c                               *
*  Author:Liang gang & Hu Xiao-qin                        *\
*  Funtion:This program is just a demostrate program      *\
*          to tell students how to use socket to          *\
*          make a tcp communation.                        *\
*  Date: 03-06-2009                                       *\
*                                                         *\
***********************************************************/


#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT 8902

int main(int argc,char** argv)
{
	struct sockaddr_in server;
	struct sockaddr_in client;
	int len;
	int port;
	int listend;
	int sockfd;
	int connectd;
	int sendnum;
	int opt;
	int recvnum;
	char send_buf[2048];
	char recv_buf[2048];

      //if (2==argc)

	port= PORT;
	memset(send_buf,0,2048);
	memset(recv_buf,0,2048);
	
    //   opt = SO_REUSEADDR;
      

      if (-1==(sockfd=socket(AF_INET,SOCK_DGRAM,0)))
      {
	 perror("create listen socket error\n");
	 exit(1);
      }
    //   setsockopt(listend,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	
	
	
      #ifdef DEBUG
	printf("the listen id is %d\n",listend);
      #endif

     memset(&server,0,sizeof(struct sockaddr_in));
	 
     server.sin_family = AF_INET;
     server.sin_addr.s_addr = htonl(INADDR_ANY);
     server.sin_port = htons(port);

     if (-1==bind(sockfd,(struct sockaddr *)&server,sizeof(struct sockaddr_in)))
     {
	perror("bind error\n");
	exit(1);

     }

    // if (-1==listen(listend,5))
    // {
	// perror("listen error\n");
	// exit(1);
    // }

    while (1)
    {
    

        // if (-1==(connectd=accept(listend,(struct sockaddr*)&client,&len)))

    	// {
		// perror("create connect socket error\n");
		// continue;
    	// }

   	#ifdef DEBUG
		printf("the connect id is %d",connect);
		printf("the client ip addr is %s",inet_ntoa(client.sin_addr));
   	#endif
	int len=(sizeof(client));
	recvnum=recvfrom(sockfd,recv_buf,sizeof(recv_buf),0,(struct sockaddr*)&client,&len);
	// cc = recvfrom(server_sock, buf, MAXBUF, 0, 
    //         (struct sockaddr *)&client_addr, &addr_size);
	sendnum = sprintf(send_buf,"hello,the guest from %s\n",inet_ntoa(client.sin_addr));

       if ( 0 >sendto(sockfd,send_buf,sendnum,0,(struct sockaddr*)&client,sizeof(client)))
	{
		perror("send error123\n");
		close(sockfd);
		continue;
	}
	
   	#ifdef DEBUG
		printf("the send num is %d",sendnum);
		printf("the client ip addr is %s",inet_ntoa(client.sin_addr));
   	#endif

    //     if (0>(recvnum = recvfrom(sockfd,recv_buf,sizeof(recv_buf),0,(struct sockaddr*)&client,&len)))
	// {
	// 	perror("recv error\n");
	// 	close(sockfd);
	// 	continue;
	// }
	// recv_buf[recvnum]='\0';

	printf ("the message from the client is: %s\n",recv_buf);

	if (0==strcmp(recv_buf,"quit"))
  {
		perror("the client break the server process\n");
		close(sockfd);
		break;
	}

	sendnum = sprintf(send_buf,"byebye,the guest from %s\n",inet_ntoa(client.sin_addr));
  
  sendto(sockfd,send_buf,sendnum,0,(struct sockaddr*)&client,sizeof(client));
  
	// close(connectd);
	continue;

   }


    close(sockfd);
    return 0;



}




