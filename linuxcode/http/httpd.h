/**************************************
*文件说明:httpd.h
*作者:段晓雪
*创建时间:Sat 05 Aug 2017 06:42:17 PM PDT
*开发环境:Centos Linux/g++ v6.5.0
****************************************/
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/epoll.h>
#include<sys/types.h>
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/sendfile.h>
#include<strings.h>
#include<sys/wait.h>

void handle_http(int sock);
int handle_simple_request(int sock,char* path,int size);
int exe_cgi(int sock,char* method,char* fullpath);
