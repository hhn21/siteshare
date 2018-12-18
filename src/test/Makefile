all: run

run: test
	./test.out

test: test.o
	gcc test.o -o test

test.o: test.c
	gcc -c test.c



clean:
	rm -f *.o *.out
