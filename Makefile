all: myencode

main.o: main.c main.h
	cc -Wall -c -o main.o main.c

Base64.o: Base64.c
	cc -Wall -c -o Base64.o Base64.c

uu.o: uu.c main.h
	cc -Wall -c -o uu.o uu.c

myencode: main.o Base64.o uu.o
	cc -o myencode main.o uu.o Base64.o


clean:
	-rm -f myencode
	-rm -f main.o uu.o Base64.o
