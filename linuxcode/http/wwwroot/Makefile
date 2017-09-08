.PHONY:all
all:BigData math gtest
BigData:BigData.cpp
	g++ -o $@ $^ -w
math:math.c
	gcc -o $@ $^ -w
gtest:gtest.cpp
	g++ gtest.cpp -o gtest -I ~/third_part/include/ -L ~/third_part/lib/ \
		-lgtest -lgtest_main -lpthread  -w
.PHONY:clean
clean:
	rm -f BigData math gtest
