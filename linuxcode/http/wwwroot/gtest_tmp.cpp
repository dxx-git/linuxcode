/**************************************
*文件说明:gtest.cpp
*作者:段晓雪
*创建时间:Sun 03 Sep 2017 02:06:42 AM PDT
*开发环境:Centos Linux/g++ v6.5.0
****************************************/
#include <iostream>
#include "BigData.h"
#include<gtest/gtest.h>
using namespace std;

void Test1()    //构造函数的测试
{
    cout<<BigData()<<endl;
    cout<<BigData(25643787278947)<<endl;
    cout<<BigData("123456789\0")<<endl;
    cout<<BigData("    +000123456789")<<endl;
    cout<<BigData("000+0000")<<endl;
    cout<<BigData("000-   123456789")<<endl;
    cout<<BigData("000+000abc123456789")<<endl;
    cout<<BigData("000-000123456789abc123456789")<<endl;
}

void Test2()//加法测试
{
    ////(同号测试)
    //BigData bd1("9999");
    //BigData bd2("+1234567");
    //cout<<bd1 + bd2<<endl;//同号都没溢出----结果没溢出
    //BigData bd3("99999999999999999999999999999999999999");
    //BigData bd4("99999999999999999999999999999999999999");
    //cout<<bd3 + bd4<<endl;//同号都溢出了----结果溢出
    //BigData bd5("9223372036854775807");
    //BigData bd6("100");
    //cout<<bd5 + bd6<<endl;//同号都没溢出----结果正溢出
    //BigData bd7("-9223372036854775808");
    //BigData bd8("-100");
    //cout<<bd7 + bd8<<endl;//同号都没溢出----结果负溢出

    //（异号测试）
	
    BigData bd1("-9999");
    BigData bd2("+1234567");
    cout<<bd1 + bd2<<endl;//异号都没溢出----结果没溢出
    BigData bd3("-99999999999999999999999999999999999999");
    BigData bd4("+11111111111111111111111111111111111111");
    cout<<bd3 + bd4<<endl;//异号都溢出了----结果溢出
    BigData bd5("-9999999999999999999999999999999");
    BigData bd6("1000");
    cout<<bd5 + bd6<<endl;//异号负数溢出----结果负溢出
    BigData bd7("-1000");
    BigData bd8("+9999999999999999999999999999999");
    cout<<bd7 + bd8<<endl;//异号正数溢出----结果正溢出
    BigData bd9(" -1111111111111111111111111111111");
    BigData bd10("+9999999999999999999999999999999");
    cout<<bd9 + bd10<<endl;//异号正数溢出----结果正溢出
}
void Test3()//减法测试
{
    ////(同号测试)
    //BigData bd1("+999999999");
    //BigData bd2("+1234567");
    //cout<<bd1 - bd2<<endl;//同号都没溢出----结果没溢出
    //BigData bd3("99999999999999999999999999999999999999");
    //BigData bd4("11111111111111111111111111111111111111");
    //cout<<bd3 - bd4<<endl;//同号都溢出了----结果溢出
    //BigData bd5("777777777777777777777777777");
    //BigData bd6("1000");
    //cout<<bd5 - bd6<<endl;//同号正数溢出----结果没溢出
    //BigData bd7("-1000");
    //BigData bd8("-88888888888888888888888888888888");
    //cout<<bd7 + bd8<<endl;//同号负数溢出----结果溢出

    //（异号测试）
    BigData bd1("-9999");
    BigData bd2("+1234567");
    cout<<bd1 - bd2<<endl;//异号都没溢出----结果没溢出
    BigData bd3("-99999999999999999999999999999999999999");
    BigData bd4("+11111111111111111111111111111111111111");
    cout<<bd3 - bd4<<endl;//异号都溢出了----结果溢出
    BigData bd5("-9999999999999999999999999999999");
    BigData bd6("1000");
    cout<<bd5 - bd6<<endl;//异号负数溢出----结果负溢出
    BigData bd7("-1000");
    BigData bd8("+9999999999999999999999999999999");
    cout<<bd7 - bd8<<endl;//异号正数溢出----结果正溢出
    BigData bd9(" -1111111111111111111111111111111");
    BigData bd10("+9999999999999999999999999999999");
    cout<<bd9 - bd10<<endl;//异号都溢出了----结果溢出
}

void Test4()//乘法测试
{
    //(同号测试)
    BigData bd1("+999999999");
    BigData bd2("+111111");
    cout<<bd1 * bd2<<endl;//同号都没溢出----结果没溢出
    BigData bd3("-99999999999999999999999999999999999999");
    BigData bd4("-11111111111111111111111111111111111111");
    cout<<bd3 * bd4<<endl;//同号都溢出了----结果溢出
    BigData bd5("9999999999999");
    BigData bd6("9999999999999");
    cout<<bd5 * bd6<<endl;//同号都没溢出----结果正溢出
    //异号测试
    BigData bd7("-1000");
    BigData bd8("7654321");
    cout<<bd7 * bd8<<endl;//异号都没溢出----结果没溢出
	BigData bd9("-9999999999999999999999999999");
    BigData bd10("+1000000000");
    cout<<bd9 * bd10<<endl;//异号溢出了----结果负溢出
    BigData bd11("9999999999999");
    BigData bd12("-9999999999999");
    cout<<bd11 * bd12<<endl;//同号都没溢出----结果负溢出
}

void Test5()//测试除法
{
    BigData bd1("+999999999");
    BigData bd2("+11111111111111");
    cout<<bd1 / bd2<<endl;//左操作数小于有操作数----结果为0
    BigData bd3("0");
    BigData bd4("-11111111111111111");
    cout<<bd3 / bd4<<endl;//左操作数--是0--结果为0
    BigData bd5("9999999999999");
    BigData bd6("9999999999999");
    cout<<bd5 / bd6<<endl;//同号且绝对值相等----结果为1
    BigData bd7("-9999999999999");
    BigData bd8("9999999999999");
    cout<<bd7 / bd8<<endl;//异号且绝对值相等----结果为-1
    BigData bd9("-9999999999999999999999999999");
    BigData bd10("+1");
    cout<<bd9 / bd10<<endl;//右操作数为1---结果为左操作数
    BigData bd11("9999999999999");
    BigData bd12("-1");
    cout<<bd11 / bd12<<endl;//右操作数为-1---结果为 -左操作数
    BigData bd13("-9999999999999999999999999999");
    BigData bd14("+100000000");
    cout<<bd13 / bd14<<endl;//异号且至少有一个溢出
    BigData bd15("9999999999999");
    BigData bd16("100000");
    cout<<bd15 / bd16<<endl;//同号且都没有溢出
}

int main()
{
	Test1();
	Test2();
	Test3();
	Test4();
	Test5();
	return 0;
}
