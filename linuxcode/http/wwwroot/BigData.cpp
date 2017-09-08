/**************************************
*文件说明:bigdata.cpp
*作者:段晓雪
*创建时间:Wed 16 Aug 2017 01:29:59 AM PDT
*开发环境:Centos Linux/g++ v6.5.0
****************************************/
#include<stdio.h>
#include<iostream>
#include<string.h>
#include"BigData.h"

int main()
{	
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
	if(num2 == (char*)1)
	{
		//参数错误
		return 0;
	}
	
	cout<<"<html>";
	cout<<"<body>";
	BigData str1(num1);
	BigData str2(num2);
	
	cout<<"<p>"<<"num1="<<str1<<"      "<<"num2="<<str2<<"</p>";
	cout<<"<p>"<<str1<<" + "<<str2<<" = "<<str1+str2<<"</p>";
	cout<<"<p>"<<str1<<"  -  "<<str2<<" = "<<str1-str2<<"</p>";
	cout<<"<p>"<<str1<<"  *  "<<str2<<" = "<<str1*str2<<"</p>";
	if(num2!=NULL)
		cout<<"<p>"<<str1<<"  /  "<<str2<<" = "<<str1/str2<<"</p>";
	else
		cout<<"<p>"<<"error:num2==0"<<"</p>";
	cout<<"</body>";
	cout<<"</html>";

	return 0;
}
