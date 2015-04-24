PEX4: LRUstack.o PEX4Template.o makefile
	gcc LRUstack.o PEX4Template.o -o PEX4 -ggdb -Wall

PEX4Template.o: PEX4Template.c byutr.h LRUstack.h makefile
	gcc -c PEX4Template.c -ggdb -Wall

LRUstack.o: LRUstack.c LRUstack.h makefile
	gcc -c LRUstack.c -ggdb -Wall

clean:
	-rm *.o
	-rm *~
	-rm PEX4
	clear