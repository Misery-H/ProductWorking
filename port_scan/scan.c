
#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT 8900
#define ADDR "127.0.0.1"
#define MAX  65535
#define MIN  0
void print_usage(char * cmd)
{
	fprintf(stderr," %s usage:\n",cmd);
	fprintf(stderr,"%s IP_Addr [port]\n",cmd);

}
int scan(int port,char* addr){
    
    struct sockaddr_in server;
	int ret;
    int sockfd;
    	if (-1==(sockfd=socket(AF_INET,SOCK_STREAM,0)))
	{
		perror("can not create socket\n");
		exit(1);
	}
    
	memset(&server,0,sizeof(struct sockaddr_in));
    
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(addr);
	server.sin_port = htons(port);
    ret=connect(sockfd,(struct sockaddr*)&server,sizeof(struct sockaddr));
    
    close(sockfd);
    
    return ret;
}

int main(int argc,char** argv)
{
	int ret;
    int port_min,port_max;
	int sockfd;
    struct servent* sptr;
    char* addr;
    

    if(argc<2){
        port_min=MIN;
        port_max=MAX;
        addr=ADDR;
    }
    else if (argc==2)
    {   
        port_min=MIN;
        port_max=MAX;
        addr=argv[1];
    }else if (argc==4)
    {
        port_min=atoi(argv[2]);
        port_max=atoi(argv[3]);
        addr=argv[1];
    }else
    {
        print_usage(argv[0]);
		exit(1);
    }
	while(port_min<=port_max){
        
        if(scan(port_min,addr)==0){
            // printf("in\n");
            printf("%d:",port_min);
            sptr = getservbyport(htons(port_min),NULL);
            // printf("in\n");
            // puts(sptr->s_name);
            if (sptr==NULL)
            {
                printf("UNKNOWN\n");
            }
            else{printf("%s\n",sptr->s_name);}
            
        }
        port_min++;
    }
   


}




