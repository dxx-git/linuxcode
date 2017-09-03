#include<iostream>
#include<gtest/gtest.h>
using namespace std;

void Fun()
{
	char* p = NULL;
	*p = 10;
}
TEST(FunDeathTest,NullPointer)
{
	EXPECT_DEATH(Fun(),"");
}

TEST(ExitDeathTest, Demo)
{
    EXPECT_EXIT(_exit(1),  testing::ExitedWithCode(1),  "");
}
