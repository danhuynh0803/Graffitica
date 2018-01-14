CPP = g++
LIBS = -lboost_filesystem -lboost_system
BOOSTDIR = L/usr/lib/x86_64-linux-gnu/ 
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
	$(CPP) $(CPPFLAGS) -c $(LIBS) canvas.cpp 

main: main.o
	$(CPP) $(CPPFLAGS) -c $(LIBS) main.cpp

build: 
	$(CPP) $(CPPFLAGS) $(OFLAGS) $(BUILDEXE) canvas.o main.o $(LIBS)

clean: 
	$(RM) *.o graffitica

clean_out: 
	$(RM) *.ppm
