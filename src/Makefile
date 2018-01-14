CPP = g++
LIBS = graff.o shapes.o canvas.o main.o 
BOOSTDIR = L/usr/lib/x86_64-linux-gnu/ 
BOOST = -lboost_filesystem -lboost_system
CPPFLAGS = -lstdc++ -std=c++11 -g
OFLAGS = -o


ifeq ($(OS), Windows_NT)
	BUILDEXE := graffitica.exe
	RM := del
else 
	BUILDEXE := graffitica 
	RM := rm -rf	
endif

all: graff shapes canvas main build

graff: graff.o
	$(CPP) $(CPPFLAGS) -c $(BOOST) graff.cpp 

shapes: shapes.o
	$(CPP) $(CPPFLAGS) -c $(BOOST) shapes.cpp 

canvas: canvas.o
	$(CPP) $(CPPFLAGS) -c $(BOOST) canvas.cpp 

main: main.o
	$(CPP) $(CPPFLAGS) -c $(BOOST) main.cpp

build: 
	$(CPP) $(CPPFLAGS) $(OFLAGS) $(BUILDEXE) $(LIBS) $(BOOST)

clean: 
	$(RM) *.o graffitica

clean_out: 
	$(RM) *.ppm
