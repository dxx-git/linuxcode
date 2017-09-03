#include<iostream>
#include<gtest/gtest.h>
using namespace std;

int Add(int x,int y)
{
	return x + y;
}

//全局的事件机制
class AddEnvironment:public testing::Environment
{
public:
	virtual void SetUp()
	{
		cout<< "AddEnvironment::SetUp" <<endl;
	}
	virtual void TearDown()
	{
		cout<< "AddEnvironment::TeanDown" <<endl;
	}
};
TEST(AddTest,Environment)
{
	ASSERT_EQ(Add(1,2),3)<<"ok";
	ASSERT_NE(Add(1,2),3)<<"failed";
}

//TestCase
class TestCase:public testing::Test
{
protected:
	static void SetUpTestCase()
	{
		cout<<"TestCase: SetUpTestCase"<<endl;
		_p = new int;
	}
	static void TearDownTestCase()
	{
		cout<<"TestCase: TearDownTestCase"<<endl;
		delete _p;
		_p = NULL;
	}

static int* _p;
};
int* TestCase::_p = 0;
TEST_F(TestCase,Test1)
{
	ASSERT_EQ(Add(3,4),7)<<"ok";
	ASSERT_NE(Add(3,4),7)<<"failed";
}
TEST_F(TestCase,Test2)
{
	ASSERT_EQ(Add(5,6),11)<<"ok";
	ASSERT_NE(Add(5,6),11)<<"failed";
}

//Test
class MyTest:public testing::Test
{
protected:
	virtual void SetUp()
	{
		//_test.Init();
		cout<<"Test: SetUp"<<endl;
	}
	virtual void TearDown()
	{
		//_test.Destroy();
		cout<<"Test: TearDown"<<endl;
	}

 //class A _test;
};
TEST_F(MyTest,Test1)
{
	ASSERT_EQ(Add(7,8),15)<<"ok";
	ASSERT_NE(Add(7,8),15)<<"failed";
}
TEST_F(MyTest,Test2)
{
	ASSERT_EQ(Add(9,10),19)<<"ok";
	ASSERT_NE(Add(9,10),19)<<"failed";
}

int main(int argc,char* argv[])
{
	testing::AddGlobalTestEnvironment(new AddEnvironment);//将全局Add事件挂起来,可有事件
	testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}

