all:
	$(CXX) -std=c++11 -Wall -Wextra -Werror forward_to_member_test.cpp
check: all
	./a.out
	bash negative_test
