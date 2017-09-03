/**************************************
*文件说明:epoll_server.c
*作者:段晓雪
*创建时间:Sat 29 Jul 2017 09:02:45 PM PDT
*开发环境:Kali Linux/g++ v6.3.0
****************************************/

#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<fcntl.h>

typedef struct fd_buff
{
	int fd;
	char buf[4096];
}fd_buff,*fd_buff_p;

static fd_buff_p alloc_buff(int sock)
{
	fd_buff_p temp = (fd_buff_p)malloc(sizeof(fd_buff));
	if(!temp)
	{
		perror("malloc");
		return NULL;
	}
	temp->fd = sock;
	return temp;
}
static void del_fd_buff(fd_buff_p ptr)
{
	if(ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}
int set_fd_nonblock(int fd)
{
	int fl = fcntl(fd,F_GETFL);
	if(fl < 0)
	{
		perror("fcntl");
		exit(6);
	}
	fcntl(fd,F_SETFL,fl | O_NONBLOCK);
}
int myread(int fd,char* buf)//假设buf够用
{
	ssize_t len = 0;
	ssize_t total = 0;	
	
	//为了方便测试，每次只让读8个字节，实际运用中应该比较大
	while((len=read(fd,buf+total,8)) > 0 && len == 8)
	{
		total += len;//读成功并且符合预期
	}
	if(len > 0 && len < 8)//没有读到8个字节，更新total
		total += len;
	
	return total;
}
int mywrite(int fd,char* buf,int size)
{
	ssize_t len = 0;
	ssize_t total = 0;	
	
	//为了方便测试，每次只让写8个字节，实际运用中应该比较大
	while(size > 0 && (len=write(fd,buf+total,8)) > 0 && len == 8)
	{
		total += len;//写成功并且符合预期
		size -= len;
	}
	if(len > 0 && len < 8)//没有写到8个字节，更新total
		total += len;
	
	return total;
}
int startup(char* ip,int port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("socket");
		exit(1);
	}

	//端口号复用
	int opt = 1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = inet_addr(ip);
	local.sin_port = htons(port);
	if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)
	{
		perror("bind");
		exit(2);
	}

	if(listen(sock,10) < 0)
	{
		perror("listen");
		exit(3);
	}
	return sock;
}

int main(int argc,char* argv[])
{
	if(argc != 3)
	{
		printf("Usage:%s [local_ip] [local_port]\n");
		return 1;
	}
	int listen_sock = startup(argv[1],atoi(argv[2]));
	printf("listen_sock:%d\n",listen_sock);

	int epfd = epoll_create(256);//创建一个epoll的句柄
	if(epfd < 0)
	{
		perror("epoll_create");
		close(listen_sock);
		exit(4);
	}
	
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	//ev.data.fd = listen_sock;
	ev.data.ptr = alloc_buff(listen_sock);

	epoll_ctl(epfd,EPOLL_CTL_ADD,listen_sock,&ev);//注册新的fd到epfd中
	
	while(1)
	{
		struct epoll_event revs[32];
		int size = 32;
		//int timeout = 2000;//设置超时时间为2秒  -1:阻塞   0:不阻塞
		int timeout = -1;

		int nums = 0;
		switch(nums = epoll_wait(epfd,revs,size,timeout))
		{
			case -1:
				printf("perror");
				break;
			case 0:
				printf("timeout\n");
				break;
			default:
			{
				//at least one ev readly!
				int i = 0;
				for(; i < nums; i++)
				{
					//int fd = revs[i].data.fd;
					int fd = ((fd_buff_p)revs[i].data.ptr)->fd;
					//listen_sock readly and need write
					if(fd == listen_sock && revs[i].events & EPOLLIN)
					{
						struct sockaddr_in client;
						int len = sizeof(client);
						int new_sock = accept(listen_sock,\
								(struct sockaddr*)&client,&len);
						if(new_sock < 0)
						{
							perror("accept");
							exit(5);
						}
						printf("get a client:[%s %d]\n",\
							inet_ntoa(client.sin_addr),ntohs(client.sin_port));
						ev.events = EPOLLIN | EPOLLET;//设置文件描述符可读
						
						set_fd_nonblock(new_sock);//设置非阻塞

						//ev.data.fd = new_sock;
						ev.data.ptr = alloc_buff(new_sock);
						epoll_ctl(epfd,EPOLL_CTL_ADD,new_sock,&ev);
					}//if
					else if(fd != listen_sock)
					{
						//fd read readly!
						if(revs[i].events & EPOLLIN)//普通事件到达并进行了读
						{
							//char buf[1024];
							//ssize_t s = read(fd,buf,sizeof(buf)-1);//bug
							char* buf = ((fd_buff_p)(revs[i].data.ptr))->buf;
							while(1){
							ssize_t s = myread(fd,buf);
							if(s > 0)
							{
								buf[s-1] = 0;
								printf("from client:%s\n",buf);
								//修改事件的状态并将其添加到就绪队列中去
								ev.events = EPOLLOUT;
								epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&ev);
								break;
							}	
							else if(s == 0)
							{
								printf("client is quit!!!\n");
								epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
								close(fd);
								break;
							}
							else
							{
								perror("read");
								del_fd_buff(revs[i].data.ptr);
								epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
								close(fd);
								break;
							}
							}
						}//else if
						else if(revs[i].events & EPOLLOUT)
					    //fd write readly!普通事件发生了写事件
						{
							char* msg = "hello world\n";
							//ssize_t ret = write(fd,msg,strlen(msg));
							ssize_t ret = mywrite(fd,msg,strlen(msg));
							if(ret < 0)
							{
								perror("write");
								exit(6);
							}
							del_fd_buff(revs[i].data.ptr);
							epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL); close(fd); }//else if //other ev readly!
						else//其他事件不做处理 
						{}
					}

				}
			}//default
		}//switch
	}//while
	close(listen_sock);
	return 0;
}
