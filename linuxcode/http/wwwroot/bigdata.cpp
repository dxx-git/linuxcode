/**************************************
*文件说明:bigdata.cpp
*作者:段晓雪
*创建时间:Wed 16 Aug 2017 01:29:59 AM PDT
*开发环境:Centos Linux/g++ v6.5.0
****************************************/
#include<stdio.h>
#include<string.h>
//#include"bigdata.h"
int main(){
	
	char *num1;
	char *num2;
	char buff[4096];

	ssize_t s = read(0,buff,sizeof(buff)-1);
	buff[s] = 0;
	num1 = strchr(buff,'=') + 1;
	if(num1 == (char*)1){
		//参数错误
		return 0;
	}
	char *tmp = strchr(num1,'&');
	if(tmp == NULL){
		//参数错误
		return 0;
	}
	*tmp = '\0';
	num2 = strchr(tmp+1,'=') + 1;
	if(num2 == (char*)1){
		//参数错误
		return 0;
	}
	
	
	
	printf("<html>");
	printf("<body>");
	printf("%s\n",num1);	
	printf("%s\n",num2);	
	printf("</body>");
	printf("</html>");

	return 0;
}
