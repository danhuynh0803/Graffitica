CPP = g++
CPPFLAGS = -lstdc++ -std=c++11 -g
OFLAGS = -o

ifeq ($(OS), Windows_NT)
	BUILDEXE := graffitica.exe
	RM := del
else 
	BUILDEXE := graffitica 
	RM := rm -rf	
endif

all: canvas main build

canvas: canvas.o
	$(CPP) $(CPPFLAGS) -c canvas.cpp 

main: main.o
	$(CPP) $(CPPFLAGS) -c main.cpp 

build: 
	$(CPP) $(CPPFLAGS) $(OFLAGS) $(BUILDEXE) canvas.o main.o

clean: 
	$(RM) *.o graffitica

clean_out: 
	$(RM) *.ppm
