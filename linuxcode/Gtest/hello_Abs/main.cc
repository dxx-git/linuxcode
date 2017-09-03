#include<iostream>
#include<gtest/gtest.h>
using namespace std;

//TEST(TestCaseName,TestName)
//{
//	cout<<"hello world"<<endl;	
//}

int Abs(int x)
{
	return x > 0 ? x:-x;
}

TEST(AbsTest,Positive)
{
	ASSERT_EQ(Abs(1),1)<<"Test ok";
	ASSERT_NE(Abs(1),1)<<"Test failed";
}

TEST(AbsTest,Negative)
{
	ASSERT_EQ(Abs(-1),1)<<"Test ok";
	ASSERT_NE(Abs(-1),1)<<"Test failed";
}

TEST(AbsTest,Zero)
{
	ASSERT_EQ(Abs(0),0)<<"Test ok";
	ASSERT_NE(Abs(0),0)<<"Test failed";
}

int main(int argc,char* argv[])
{
	testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}


