CPP = g++
CPPFLAGS = -std=c++11
INCLUDE = -I../src/

TESTS = tests-vec.cpp tests-matrix.cpp

.PHONY: all clean

all: tests-main tests

tests-main: tests-main.o
	$(CPP) $(CPPFLAGS) tests-main.cpp -c

tests: $(TESTS)
	$(CPP) $(CPPFLAGS) tests-main.o $(INCLUDE) $(TESTS) -o tests && ./tests -r compact

clean:
	rm -rf tests *.o
