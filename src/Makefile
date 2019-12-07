CPP = g++
CPPFLAGS = -lstdc++ -std=c++11 -g
OFLAGS = -o
OBJECTS = shapes.o canvas.o model.o main.o

BOOSTDIR = L/usr/lib/
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

.PHONY: all clean

all: $(OBJECTS) build

build: $(OBJECTS)
	$(CPP) $(CPPFLAGS) $(OFLAGS) $(BUILDEXE) $(OBJECTS) $(BOOST)

clean:
	$(RM) *.o graffitica
