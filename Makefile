all:
	$(CXX) -std=c++11 -Wall -Wextra -Werror forward_to_member_test.cpp

coverage:
	$(CXX) -std=c++11 -Wall -Wextra -Werror -fprofile-arcs -ftest-coverage forward_to_member_test.cpp -lgcov
	./a.out
	bash negative_test

check: all
	./a.out
	bash negative_test

clean:
	rm a.out *.gcda *.gcno 2>/dev/null || true
