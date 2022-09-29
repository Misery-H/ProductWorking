#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>

#define IP "127.0.0.1"
#define PORT 8902
#define BUFSIZE 2048
void print_err(char *str, int line, int err_no) {
	printf("%d, %s :%s\n",line,str,strerror(err_no));
	_exit(-1);
}
int my_popen(char* cmd,char* msg){
    int f_des[2];
	pid_t pid;
	if (-1==pipe(f_des)){
		perror("create pipe error\n");
		exit(1);
	}
	pid = fork();
	if(0>pid){
		perror("fork error\n");
		exit(1);
	}
	else if (0<pid){
		close(f_des[1]);
		wait();
		if (-1==(read(f_des[0],msg,BUFSIZE))){
			perror("read pipe data error");
			exit(1);
		}
	}
	else{
		close(f_des[0]);

		dup2(f_des[1],STDOUT_FILENO);
		
    if (-1==(execlp(cmd,cmd,(char*)NULL))){
			perror("execvp error\n");
			exit(0);
		}
		exit(0);
	}
}
int execute(char* cmd,int sockfd)
{
    char msg[BUFSIZE];
    char* errbuf="command cannot execute\n";
    int ret;
    int counter;
    counter=0;
    memset(msg,0,BUFSIZE);
    my_popen(cmd,msg);
    ret=send(sockfd,msg,strlen(msg),0);
    if(0>ret)
        return -1;
    return 0;

}

void *receive(void *pth_arg) {
	int ret = 0;
	long cfd = (long)pth_arg;
	char buf[BUFSIZE] = {0};
	while(1) {
		bzero(&buf, sizeof(buf));
		ret = recv(cfd, &buf, sizeof(buf),0);	
		if (-1 == ret) {
			print_err("recv failed",__LINE__,errno);
		}
		else if (ret > 0)
			printf("command : %s \n",buf);
               if (strstr(buf,"quit"))
  		{
		perror("the client break the server process\n");
		send(cfd,"quit successfully",18,0);
		close(cfd);
		break;
	}
        int l=strlen(buf);
        printf("%d\n",l);
        buf[strlen(buf)-1]='\0';
        printf("command2 : %s \n",buf);
		execute(buf,cfd); 
	}
}

int main()
{
	int skfd = -1, ret = -1;
	skfd = socket(AF_INET, SOCK_STREAM, 0);
	if ( -1 == skfd) {
		print_err("socket failed",__LINE__,errno);
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET; 
	addr.sin_port = htons(PORT); 
	addr.sin_addr.s_addr = inet_addr(IP); 

	ret = bind(skfd, (struct sockaddr*)&addr, sizeof(addr));
	if ( -1 == ret) {
		print_err("bind failed",__LINE__,errno);
	}
 
	ret = listen(skfd, 3);
	if ( -1 == ret ) {
		print_err("listen failed", __LINE__, errno);
	}
	
	long cfd = -1;
	pthread_t id;
	while (1) {
		struct sockaddr_in caddr = {0};
		int csize = sizeof(caddr);
		cfd = accept(skfd, (struct sockaddr*)&caddr, &csize);
		if (-1 == cfd) {
			print_err("accept failed", __LINE__, errno);
		}

		printf("get connect from cport = %d and caddr = %s\n", ntohs(caddr.sin_port),inet_ntoa(caddr.sin_addr));
	

		int ret = pthread_create(&id,NULL,receive,(void*)cfd);
		if(-1 == ret) print_err("accept failed", __LINE__, errno); 
	}
	return 0;
}

