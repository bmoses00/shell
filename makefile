all: main.o parse.o executes.o
	gcc -o program main.o parse.o executes.o

main.o: main.c parse.h executes.h
	gcc -c -g main.c

parse.o: parse.c parse.h
	gcc -c -g parse.c

run:
	./program

clean:
	rm program
	rm *.o
