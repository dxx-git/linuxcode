#include<iostream>
#include<gtest/gtest.h>
using namespace std;

int Mul(int x,int y)
{
	return x * y;
}

//ASSERT
TEST(MulTest,ASSERT)
{
	ASSERT_TRUE(Mul(2,4))<< "assert:2*4!=0";
	
	ASSERT_TRUE(Mul(3,0))<< "assert:3*0!=0";
	
	ASSERT_FALSE(Mul(5,7))<< "assert:5*7==0";
	
	ASSERT_FALSE(Mul(0,0))<< "assert:0*0==0";
}

//EXPECT
TEST(MulTest,EXPECT)
{
	EXPECT_TRUE(Mul(2,4))<< "expect:2*4!=0";
	
	EXPECT_TRUE(Mul(3,0))<< "expect:3*0!=0";

	EXPECT_FALSE(Mul(5,7))<< "expect:5*7==0";
	
	EXPECT_FALSE(Mul(0,0))<< "expect:0*0==0";
}

//data check
TEST(DataTest,TestName)
{
	EXPECT_EQ(1,1);
	EXPECT_NE(1,2);
	EXPECT_GT(3,2);
	EXPECT_LT(0,1);
	EXPECT_GE(2,4);
	EXPECT_LE(0,0);
}

//string check
TEST(StringTest,TestName)
{
	EXPECT_EQ("abc","ABC");
	EXPECT_NE("ABC","ABC");
}
