/**************************************
*文件说明:httpd.c
*作者:段晓雪
*创建时间:Sat 05 Aug 2017 06:41:58 PM PDT
*开发环境:Centos Linux/g++ v6.5.0
****************************************/
#include"httpd.h"

const char * respond_line = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n";//状态行
const char * blank_line = "\r\n";//空行

static int get_line(int sock,char* buff,int size)//读取一行的内容
{
	int index = 0;
	char ch = 0;
	while(index < size-1 && ch != '\n')
	{
		ssize_t s = recv(sock,&ch,1,0);//一个字符一个字符读，将读到的字符存在ch当中
		if(s > 0)
		{
			if(ch == '\r')
			{
				recv(sock,&ch,1,MSG_PEEK);//查看下一个字符是不是'\n',但不读走它
				if(ch == '\n')
					recv(sock,&ch,1,0);//ch='\n'
				else
					ch = '\n';//ch='\r'='\n'
			}
			buff[index++] = ch;
		}
		else
		{
			perror("recv");
			return -1;
		}
	}
	buff[index] = '\0';
	return index;
}

int handle_simple_request(int sock,char* fullpath,int size)//方法是GET并且没有传参，则简单处理
{
	//处理头部信息
	int ret = 0;
	char buff[512];
	while(ret != 1)//读到空行停止
	{
		ret = get_line(sock,buff,sizeof(buff));
	}
	
	send(sock,respond_line,strlen(respond_line),0);//发送状态行	
	send(sock,blank_line,strlen(blank_line),0);	//发送空行
	
	int fd = open(fullpath,O_RDONLY);//以只读方式打开请求的文件即可
	if(fd < 0)//可打开的文件已经达上限
	{
		perror("open");
		return 404;//500
	}
	if(sendfile(sock,fd,NULL,size) < 0)//将打开的文件响应给客户端
	{
		perror("sendfile");
		return 404;
	}
	close(fd);
	return 200;
}

int exe_cgi(int sock,char* method,char* fullpath)
{
//	printf("handle exe_cgi\n");
	char* query_string;
	char buff[1024];
	if(strcasecmp(method,"GET") == 0)
	{
		char* temp = fullpath;
		while(*temp != '?')
		{
			++temp;
		}
		*temp = '\0';
		query_string  = ++temp;
		int ret = 999;
		while((ret=get_line(sock,buff,sizeof(buff)))!=1)
		{
			;
		}
	}
	else//POST
	{
		int length = 0;
		int ret = 0;
		while((ret = get_line(sock,buff,sizeof(buff))) > 0)
		{
			if(ret == 1)//说明读到空行了
				break;
			if(strncasecmp(buff,"Content-Length: ",16)==0)
			{
			//	printf("%s",buff);
				char* tmp = strchr(buff,' ');
			//	printf("tmp=%s",tmp);
				if(NULL == tmp)
					return 404;
				length = atoi(tmp+1);
			//	printf("length=%d\n",length);
			}
			else
			{
				printf("%s",buff);
			}
		}
		ssize_t s = recv(sock,buff,length,0);
		buff[s] = '\0';
		query_string = buff;
	}
	printf("path:%s\n",fullpath);
	printf("query_string:%s\n",query_string);

	char method_env[128];
	char query_string_env[128];
	sprintf(method_env,"REQUEST_METHOD=%s",method);
	sprintf(query_string_env,"QUERY_STRING=%s",query_string);
	//printf("%s\n",method_env);
	//printf("%s\n",query_string_env);
	putenv(method_env);
	putenv(query_string_env);

	int read_fd[2];		//for child
	int write_fd[2];	//for child
	//printf("pipe1\n");
	if(pipe(read_fd) < 0)
	{
		perror("pipe");
		return 404;
	}
	//printf("pipe2\n");
	if(pipe(write_fd) < 0){
		perror("pipe");
		return 404;
	}
	pid_t pid = fork();
	if(pid == 0){	//child
		close(read_fd[1]);
		close(write_fd[0]);
		dup2(read_fd[0],0);
		dup2(write_fd[1],1);
		execl(fullpath,fullpath,NULL);
		exit(-1);
	}
	else if(pid > 0)//father
	{	
		close(read_fd[0]);
		close(write_fd[1]);
		write(read_fd[1],query_string,strlen(query_string));
		wait(NULL);
		ssize_t ret = read(write_fd[0],buff,sizeof(buff)-1);
		buff[ret] = 0;
		send(sock,respond_line,strlen(respond_line),0);
		send(sock,blank_line,strlen(blank_line),0);
		send(sock,buff,ret,0);
		printf("father recive data:%s\n",buff);
		close(read_fd[1]);
		close(write_fd[0]);
	}
	else
	{
		printf("fork fail\n");
		return 404;
	}

	return 200;
}

void error_responce(int sock,int err_code)
{
	return;
}

void handle_http(int sock)//处理http请求
{
	char buff[512];
	int err_code = 200;
	int cgi = 0;

	if(get_line(sock,buff,sizeof(buff)) <= 0)//按行读失败
	{
		err_code = 404;
		goto end;
	}
	printf("%s",buff);//将读到的请求行打印出来

	// GET /a/b/c.html HTTP/1.0
	char* method = buff;//method:GET or POST
	char* path;
	char* temp = buff;
	while(*temp != ' ')//提取方法
	{
		++temp;
	}
	*temp++ = '\0';
	while(*temp == ' ')//将多余的空格去掉
	{
		++temp;	
	}

	if((strcasecmp(method,"GET")!=0) && (strcasecmp(method,"POST")!=0))//方法不合法
	{
		printf("method error\n");
		err_code = 404;
		goto end;
	}
	printf("method:%s\n",method);
		
	//get url
	path = temp;
	while(*temp != ' ')//提取路径
	{
		++temp;
	}
	*temp = '\0';
	//temp = strchr(path,'\0');//在路径里面查找字符'\0'
	if(*(temp-1) == '/')//查看路径是不是以'/'结尾，如果是在后面追加首页
		strcat(temp,"index.html");
	printf("path:%s\n",path);//打印请求的路径
	
	char fullpath[128];//完整路径
	sprintf(fullpath,"wwwroot%s",path);//将所有请求文件放在wwwroot目录下
    
	struct stat info;
	if(strcasecmp(method,"GET")==0 && strchr(path,'?')==NULL)//GET没传参,处理简单请求	
	{
		if(stat(fullpath,&info) < 0)//判断路径下的文件是否存在
		{
			printf("file not exist\n");
			err_code = 404;
			goto end;
		}
		if(S_ISDIR(info.st_mode))//判断路径是否是目录
		{
			sprintf(fullpath,"%s/index.html",fullpath);//如果是目录则给目录追加首页
		}	
		else if((S_IXOTH & info.st_mode)
			 || (S_IXUSR & info.st_mode)
			 || (S_IXGRP & info.st_mode))//具有执行权限
		{
			cgi = 1;//exe_cgi
		}
	}	
	else
	{
		cgi = 1;
	}

	//path not cgi
	if(cgi == 0)
	{
		err_code = handle_simple_request(sock,fullpath,info.st_size);
	}
    else//cgi == 1
	{
		err_code = exe_cgi(sock,method,fullpath);
	}

end:
	if(err_code != 200)
	{
		error_responce(sock,err_code);
	}
	close(sock);
}
