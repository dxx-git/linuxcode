/**************************************
*文件说明:socketpair.c
*作者:段晓雪
*创建时间:Wed 26 Jul 2017 08:30:06 AM PDT
*开发环境:Kali Linux/g++ v6.3.0
****************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>

int main()
{
	int fd[2];
	int ret = socketpair(PF_LOCAL,SOCK_STREAM,0,fd);
	if(ret < 0)
	{
		perror("socketpair");
		return 1;
	}
	pid_t pid = fork();
	if(pid < 0)
	{
		perror("fork");
		return 2;
	}
	else if(pid == 0)//child
	{
		close(fd[0]);
		char buf[1024];
		while(1)
		{
			ssize_t s = read(fd[1],buf,sizeof(buf)-1);
			if(s > 0)
			{
				buf[s] = '\0';
				char* msg = "i am your child";
				write(fd[1],msg,strlen(msg));
				printf("father say to child:%s\n",buf);
			}
		}
	}
	else
	{
		close(fd[1]);
		while(1)
		{
			char* msg = "i am your father";
			char buf[1024];
			write(fd[0],msg,strlen(msg));
			ssize_t s = read(fd[0],buf,sizeof(buf)-1);
			if(s > 0)
			{
				buf[s] = '\0';
				printf("child say to father:%s\n",buf);
			}
			sleep(1);
		}
	}
	return 0;
}
