#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


#define BUF_SIZE 2048

void print_usage(char* cmd)
{
	fprintf(stdout,"%s usage:\n",cmd);
	fprintf(stdout,"%s command\n",cmd);
}


int main(int argc,char** argv)
{
	int f_des[2];
	pid_t pid;
	char msg[BUF_SIZE];

	if (2>argc)
	{
		print_usage(argv[0]);
		exit(1);
	}
	
	if (-1==pipe(f_des))
	{
		perror("create pipe error\n");
		exit(1);
	}

		
	pid = fork();

	if(0>pid)
	{
		perror("fork error\n");
		exit(1);
	}
	else if (0<pid)
	{
		close(f_des[1]);

		wait();
		
		if (-1==(read(f_des[0],msg,BUF_SIZE)))
		{
			perror("read pipe data error");
			exit(1);

		}
		printf("the command result is[parent]:\n");
		printf("%s \n",msg);
	}
	else
	{
		close(f_des[0]);

		dup2(f_des[1],STDOUT_FILENO);
		
    if (-1==(execvp(argv[1],&argv[1])))
		{
			perror("execvp error\n");
			exit(0);
		}
		
		exit(0);

	

	}
	


}
