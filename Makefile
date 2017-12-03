CPP = g++
CPPFLAGS = -lstdc++ -std=c++11
OFLAGS = -o

RM = rm -rf

all: canvas main build

canvas: canvas.o
	$(CPP) $(CPPFLAGS) -c canvas.cpp 

main: main.o
	$(CPP) $(CPPFLAGS) -c main.cpp 

build: 
	$(CPP) $(CPPFLAGS) $(OFLAGS) graffitica canvas.o main.o

clean: 
	$(RM) *.o graffitica
