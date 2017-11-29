CC = g++
CFLAGS = -g -std=gnu++11

all: main canvas 

canvas: 
	$(CC) $(CFLAGS) canvas.cpp -o canvas.o

main: 
	$(CC) $(CFLAGS) main.cpp -o main.o

clean: 
	rm -rf *.o
