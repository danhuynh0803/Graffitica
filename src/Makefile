CPP = g++
CPPFLAGS = -lstdc++ -std=c++11 -g
OFLAGS = -o
OBJECTS = matrix.o graff.o shapes.o canvas.o main.o 

BOOSTDIR = L/usr/lib/x86_64-linux-gnu/ 
BOOST = -lboost_filesystem -lboost_system

ifeq ($(OS), Windows_NT)
	BUILDEXE := graffitica.exe
	RM := del
else 
	BUILDEXE := graffitica 
	RM := rm -rf
endif

.SUFFIXES: .o .cpp
.cpp.o:
	$(CPP) $(CPPFLAGS) -c $(BOOST) $< $(OFLAGS) $@

.PHONY: all
all: $(OBJECTS) build 

build: $(OBJECTS)
	$(CPP) $(CPPFLAGS) $(OFLAGS) $(BUILDEXE) $(OBJECTS) $(BOOST)

.PHONY: clean
clean: 
	$(RM) *.o graffitica
