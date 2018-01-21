CPP = g++
CPPFLAGS = -std=c++11
INCLUDE = -I../src/

TESTS = tests-add.cpp tests-vec.cpp

all: tests-main tests

tests-main: tests-main.o 
	$(CPP) $(CPPFLAGS) tests-main.cpp -c

tests: $(TESTS)
	$(CPP) $(CPPFLAGS) tests-main.o $(INCLUDE) $(TESTS) -o tests && ./tests -r compact

clean: 
	rm -rf tests *.o	
