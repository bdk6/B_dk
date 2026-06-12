

all:	bi bc

codegen.o: codegen.c codegen.h definitions.h
	gcc -c codegen.c

parser.o:	parser.c definitions.h
	gcc -c parser.c

scanner.o:	scanner.h scanner.c definitions.h
	gcc -c scanner.c

strings.o:  strings.c strings.h
	gcc -c strings.c

symtable.o:	symtable.h symtable.c definitions.h
	gcc -c symtable.c

bc:	parser.o scanner.o symtable.o strings.o codegen.o
	gcc -o bc parser.o scanner.o symtable.o codegen.o

bi: bi.c definitions.h
	gcc -o bi bi.c

clean:
	rm -f *.o bc bi

