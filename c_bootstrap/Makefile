

bc:	system.o parser.o scanner.o blib.o symtable.o 
	gcc -o bc system.o parser.o scanner.o blib.o symtable.o

system.o:	system.c system.h definitions.h
	gcc system.c

parser.o:	parser.c parser.h definitions.h
	gcc parser.c

scanner.o:	scanner.c scanner.h definitions.h
	gcc scanner.c

blib.o:	blib.c blib.h definitions.h
	gcc blib.c
symtable.o:	symtable.c symtable.h definitions.h
	gcc symtable.c


