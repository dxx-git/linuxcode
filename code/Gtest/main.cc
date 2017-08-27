#include<iostream>
using namespace std;

//
//int Abs(int x)
//{
//	return x > 0 ? x:-x;
//}
//
//void Test()
//{
//	if(Abs(1) == 1)
//		std::cout<<"Test ok"<<std::endl;
//	else
//		std::cout<<"Test failed"<<std::endl;
//}

#include<gtest/gtest.h>

TEST(TestCaseName,TestName)
{
	cout<<"hello world"<<endl;	
}

//int main()
//{
//	Test();
//	return 0;
//}
