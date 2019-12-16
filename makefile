ifeq ($(DEBUG), true)
  CC = gcc -g -c
else
  CC = gcc -c
endif

all: main.o parse.o executes.o
	gcc -o program main.o parse.o executes.o

main.o: main.c parse.h executes.h
	$(CC) main.c

parse.o: parse.c parse.h
	$(CC) parse.c

executes.o: executes.c executes.h
	$(CC) executes.c
run:
	./program

clean:
	rm program
	rm *.o
