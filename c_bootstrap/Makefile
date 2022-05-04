

all:	bc bi ba

# b preprocessor
bpp:

# b linker
bl:


# b compiler
bc:	 parser.o scanner.o  symtable.o codegen.o #blib.o system.o
	gcc -o bc parser.o scanner.o  symtable.o codegen.o #blib.o system.o

# b assembler
ba:	asm_sim.o
	gcc -o ba asm_sim.o

# b interpreter
bi:	interp.o interp.h sim_common.o sim_common.h
	gcc -o bi interp.o sim_common.o

asm_sim.o:	asm_sim.c
	gcc -c asm_sim.c

interp.o:	interp.c interp.h
	gcc -c interp.c

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

codegen.o:	codegen.c codegen.h definitions.h
	gcc -c codegen.c

clean:
	rm -f *.o bc bi ba




