#include<iostream>
#include<gtest/gtest.h>
#include<map>
#include<list>
using namespace std;

class MapTest:public testing::TestWithParam<int>
{};
TEST_P(MapTest,Insert)
{
	map<int,int> m;
	int n = GetParam();//获取参数的具体值
	EXPECT_TRUE(m.insert(make_pair<int,int>(n,0)).second);
}
INSTANTIATE_TEST_CASE_P(InsertTrue,MapTest,testing::Values(3,5,13,0,27,16));


////类型参数化1
//template<typename T>
//class ListTest:public testing::Test
//{
//public:
//	typedef list<T> List;
//	static T _shared;
//	T _value;
//};
//template <typename T>
//T ListTest<T>::_shared = T();
////定义需要测试的具体类型
//typedef testing::Types<char,int,unsigned int> MyTypes;
//TYPED_TEST_CASE(ListTest,MyTypes);
////完成测试案例，在声明模版的数据类型时，使用TypeParam 
//TYPED_TEST(ListTest,DoesBlah)
//{
//	TypeParam n = this->_value;
//	n += TestFixture::_shared;
//	typename TestFixture::List values;
//	values.push_back(n);
//}


//类型参数化：官方例子
template<typename T>
class ListTest:public testing::Test
{
public:
	typedef list<T> List;
	static T _shared;
	T _value;
};
template <typename T>
T ListTest<T>::_shared = T();
TYPED_TEST_CASE_P(ListTest);

TYPED_TEST_P(ListTest, DoesBlah)
{
	TypeParam n = 0;  
}
TYPED_TEST_P(ListTest, HasPropertyA) 
{}
REGISTER_TYPED_TEST_CASE_P(ListTest, DoesBlah, HasPropertyA);
typedef testing::Types<char, int, unsigned int> MyTypes;
INSTANTIATE_TYPED_TEST_CASE_P(My, ListTest, MyTypes);












