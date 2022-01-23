SRC_TEST_FILES = $(filter-out main.cpp, $(wildcard *.cpp)) tests/*.cpp

all:
	$(CXX) *.cpp -std=c++17 -g -O3 -Werror -Wall -Wextra -pthread -pedantic -o posix

run_tests:
	./tests/tests -s -d

build_tests:
	$(CXX) $(SRC_TEST_FILES) -I tests -I . -std=c++17 -g -O3 -Werror -Wall -Wextra -pthread -pedantic -o tests/tests

clean:
	rm -f posix
	rm -f tests/tests
