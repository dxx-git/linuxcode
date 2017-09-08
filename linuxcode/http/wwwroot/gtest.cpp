/**************************************
*文件说明:gtest.cpp
*作者:段晓雪
*创建时间:Sun 03 Sep 2017 02:06:42 AM PDT
*开发环境:Centos Linux/g++ v6.5.0
****************************************/
#include <iostream>
#include<gtest/gtest.h>
#include "BigData.h"
using namespace std;

TEST(BigDataTest,Construct)    //构造函数的测试
{
	ASSERT_STREQ(BigData().GetString().c_str(),"");
    ASSERT_STREQ(BigData(25643787278947).GetString().c_str(),"+25643787278947");
    ASSERT_STREQ(BigData("123456789\0").GetString().c_str(),"+123456789");
    ASSERT_STREQ(BigData("    +000123456789").GetString().c_str(),"+123456789");
    ASSERT_STREQ(BigData("000+0000").GetString().c_str(),"+0");
    ASSERT_STREQ(BigData("000-   123456789").GetString().c_str(),"-123456789");
    ASSERT_STREQ(BigData("000+000abc123456789").GetString().c_str(),"+0");
    ASSERT_STREQ(BigData("000-000123456789abc123456789").GetString().c_str(),"-123456789");
}

TEST(BigDataTest,AddTest)//加法测试
{
    //(同号测试)
	BigData bd1("9999");
	BigData bd2("+1234567");
	ASSERT_STREQ((bd1+bd2).GetString().c_str(),"+1244566");//同号都没溢出----结果没溢出
    
	BigData bd3("99999999999999999999999999999999999999");
    BigData bd4("99999999999999999999999999999999999998");
    ASSERT_STREQ((bd3+bd4).GetString().c_str(),"+199999999999999999999999999999999999997");//同号都溢出了----结果溢出
    BigData bd5("9223372036854775807");
    BigData bd6("100");
    ASSERT_STREQ((bd5+bd6).GetString().c_str(),"+9223372036854775907");//同号都没溢出----结果正溢出
    BigData bd7("-9223372036854775808");
    BigData bd8("-100");
    ASSERT_STREQ((bd7+bd8).GetString().c_str(),"-9223372036854775908");//同号都没溢出----结果负溢出

    //（异号测试）
    BigData bd9("-9999");
    BigData bd10("+1234567");
    ASSERT_STREQ((bd9+bd10).GetString().c_str(),"+1224568");//异号都没溢出----结果没溢出
    
	BigData bd11("-99999999999999999999999999999999999999");
    BigData bd12("+11111111111111111111111111111111111111");
    ASSERT_STREQ((bd11+bd12).GetString().c_str(),"-88888888888888888888888888888888888888");//异号都溢出了----结果溢出

    BigData bd13("-9999999999999999999999999999999");
    BigData bd14("1000");
    ASSERT_STREQ((bd13+bd14).GetString().c_str(),"-9999999999999999999999999998999");//异号负数溢出----结果负溢出

    BigData bd15("-1000");
    BigData bd16("+9999999999999999999999999999999");
    ASSERT_STREQ((bd15+bd16).GetString().c_str(),"+9999999999999999999999999998999");//异号正数溢出----结果正溢出

    BigData bd17(" -1111111111111111111111111111111");
    BigData bd18("+9999999999999999999999999999999");
    ASSERT_STREQ((bd17+bd18).GetString().c_str(),"+8888888888888888888888888888888");//异号正数溢出----结果正溢出
}

TEST(BigDataTest,SubTest)//减法测试
{
    //(同号测试)
    BigData bd1("+999999999");
    BigData bd2("+1234567");
    ASSERT_STREQ((bd1-bd2).GetString().c_str(),"+998765432");//同号都没溢出----结果没溢出

    BigData bd3("99999999999999999999999999999999999999");
    BigData bd4("11111111111111111111111111111111111111");
    ASSERT_STREQ((bd3-bd4).GetString().c_str(),"+88888888888888888888888888888888888888");//同号都溢出了----结果溢出

    BigData bd5("777777777777777777777777777");
    BigData bd6("1000");
    ASSERT_STREQ((bd5-bd6).GetString().c_str(),"+777777777777777777777776777");//同号正数溢出----结果没溢出

    BigData bd7("-1000");
    BigData bd8("-88888888888888888888888888888888");
    ASSERT_STREQ((bd7+bd8).GetString().c_str(),"-88888888888888888888888888889888");//同号负数溢出----结果溢出

    //（异号测试）
    BigData bd9("-9999");
    BigData bd10("+1234567");
    ASSERT_STREQ((bd9-bd10).GetString().c_str(),"+1244566");//异号都没溢出----结果没溢出

    BigData bd11("-99999999999999999999999999999999999999");
    BigData bd12("+11111111111111111111111111111111111111");
    ASSERT_STREQ((bd11-bd12).GetString().c_str(),"-111111111111111111111111111111111111110");//异号都溢出了----结果溢出

    BigData bd13("-9999999999999999999999999999999");
    BigData bd14("1000");
    ASSERT_STREQ((bd13-bd14).GetString().c_str(),"-10000000000000000000000000000999");//异号负数溢出----结果负溢出

    BigData bd15("-1000");
    BigData bd16("+9999999999999999999999999999999");
    ASSERT_STREQ((bd15-bd16).GetString().c_str(),"-10000000000000000000000000000999");//异号正数溢出----结果正溢出

    BigData bd17(" -1111111111111111111111111111111");
    BigData bd18("+9999999999999999999999999999999");
    ASSERT_STREQ((bd17-bd18).GetString().c_str(),"-11111111111111111111111111111110");//异号都溢出了----结果溢出
}

TEST(BigDataTest,MulTest)//乘法测试
{
    //(同号测试)
    BigData bd1("+999999999");
    BigData bd2("+111111");
    ASSERT_STREQ((bd1 * bd2).GetString().c_str(),"+111110999888889");//同号都没溢出----结果没溢出
    BigData bd3("-99999999999999999999999999999999999999");
    BigData bd4("-11111111111111111111111111111111111111");
    ASSERT_STREQ((bd3 * bd4).GetString().c_str(),\
	"+1111111111111111111111111111111111111088888888888888888888888888888888888889");//同号都溢出了----结果溢出
    
	BigData bd5("9999999999999");
    BigData bd6("9999999999999");
    ASSERT_STREQ((bd5 * bd6).GetString().c_str(),"+99999999999980000000000001");//同号都没溢出----结果正溢出

    //异号测试
    BigData bd7("-1000");
    BigData bd8("7654321");
    ASSERT_STREQ((bd7 * bd8).GetString().c_str(),"-7654321000");//异号都没溢出----结果没溢出
	
	BigData bd9("-9999999999999999999999999999");
    BigData bd10("+1000000000");
    ASSERT_STREQ((bd9 * bd10).GetString().c_str(),"-9999999999999999999999999999000000000");//异号溢出了----结果负溢出
    
	BigData bd11("9999999999999");
    BigData bd12("-9999999999999");
    ASSERT_STREQ((bd11 * bd12).GetString().c_str(),"-99999999999980000000000001");//同号都没溢出----结果负溢出
}

TEST(BigDataTest,DelTest)//测试除法
{
    BigData bd1("+999999999");
    BigData bd2("+11111111111111");
    ASSERT_STREQ((bd1 / bd2).GetString().c_str(),"+0");//左操作数小于有操作数----结果为0
    
	BigData bd3("0");
    BigData bd4("-11111111111111111");
    ASSERT_STREQ((bd3 / bd4).GetString().c_str(),"+0");//左操作数--是0--结果为0
    
	BigData bd5("9999999999999");
    BigData bd6("9999999999999");
    ASSERT_STREQ((bd5 / bd6).GetString().c_str(),"+1");//同号且绝对值相等----结果为1
    
	BigData bd7("-9999999999999");
    BigData bd8("9999999999999");
    ASSERT_STREQ((bd7 / bd8).GetString().c_str(),"-1");//异号且绝对值相等----结果为-1
    
	BigData bd9("-9999999999999999999999999999");
    BigData bd10("+1");
    ASSERT_STREQ(( bd9 / bd10).GetString().c_str(),"-9999999999999999999999999999");//右操作数为1---结果为左操作数
    
	BigData bd11("9999999999999");
    BigData bd12("-1");
    ASSERT_STREQ((bd11 / bd12).GetString().c_str(),"-9999999999999");//右操作数为-1---结果为 -左操作数
  
  	BigData bd13("-9999999999999999999999999999");
    BigData bd14("+100000000");
    ASSERT_STREQ((bd13 / bd14).GetString().c_str(),"-9999999999999999999");//异号且至少有一个溢出
   
   	BigData bd15("9999999999999");
    BigData bd16("100000");
    ASSERT_STREQ((bd15 / bd16).GetString().c_str(),"+99999999");//同号且都没有溢出
}
