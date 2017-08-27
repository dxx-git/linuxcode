/**************************************
*文件说明:math.c
*作者:段晓雪
*创建时间:Wed 16 Aug 2017 01:29:59 AM PDT
*开发环境:Centos Linux/g++ v6.5.0
****************************************/
#include<stdio.h>
int main(){
	int num1,num2;
	char buff[1024];
	ssize_t s = read(0,buff,sizeof(buff)-1);
	buff[s] = 0;
	sscanf(buff,"num1=%d&num2=%d",&num1,&num2);
	printf("num1=%d num2=%d\n",num1,num2);
	printf("<html>");
	printf("<body>");
	printf("<p>%d+%d=%d</p>",num1,num2,num1+num2);
	printf("<p>%d-%d=%d</p>",num1,num2,num1-num2);
	printf("<p>%d*%d=%d</p>",num1,num2,num1*num2);
	if(num2 != 0)
		printf("<p>%d/%d=%d</p>",num1,num2,num1/num2);
	else
		printf("<p>/ error:num2==0</p>");
	if(num2 != 0)
		printf("<p>%d%%%d=%d</p>",num1,num2,num1%num2);
	else
		printf("<p>%% error:num2==0</p>");
	
	printf("</body>");
	printf("</html>");

	return 0;
}
