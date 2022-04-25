

all:	bc

bpp:

bl:
	

bc:	 parser.o scanner.o  symtable.o #blib.o system.o
	gcc -o bc parser.o scanner.o  symtable.o #blib.o system.o

ba:



#system.o:	system.c system.h definitions.h
#	gcc -c system.c

parser.o:	parser.c parser.h definitions.h
	gcc -c parser.c

scanner.o:	scanner.c scanner.h definitions.h
	gcc -c scanner.c

#blib.o:	blib.c blib.h definitions.h
#	gcc -c blib.c

symtable.o:	symtable.c symtable.h definitions.h
	gcc -c symtable.c

clean:
	rm *.o bc




