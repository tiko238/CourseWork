CC = gcc
CFLAGS = -g -std=c99 -Wall -lncurses

all: drone_simulation

drone_simulation: main.o functions.o
	$(CC) $(CFLAGS) -o drone_simulation main.o functions.o

main.o: main.c functions.h
	$(CC) $(CFLAGS) -c main.c

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c

clean:
	rm -f drone_simulation *.o
