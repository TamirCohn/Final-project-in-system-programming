assembler: main.o BinBase64.o CutPaste.o First_pass.o Identify.o Label.o preAssembler.o Second_pass.o
	gcc -g -ansi -Wall -pedantic main.o BinBase64.o CutPaste.o First_pass.o Identify.o Label.o preAssembler.o Second_pass.o -lm -o assembler

main.o: main.c Var.h Declarations.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o
BinBase64.o: BinBase64.c Var.h Declarations.h
	gcc -c -ansi -Wall -pedantic BinBase64.c -o BinBase64.o

CutPaste.o : CutPaste.c Var.h Declarations.h
	gcc -c -ansi -Wall -pedantic CutPaste.c -o CutPaste.o

First_pass.o: First_pass.c Var.h Declarations.h
	gcc -c -ansi -Wall -pedantic First_pass.c -o First_pass.o

Identify.o: Identify.c Var.h Declarations.h
	gcc -c -ansi -Wall -pedantic Identify.c -o Identify.o

Label.o: Label.c Var.h Declarations.h
	gcc -c -ansi -Wall -pedantic Label.c -o Label.o

preAssembler.o: preAssembler.c Var.h Declarations.h Identify.h
	gcc -c -ansi -Wall -pedantic preAssembler.c -o preAssembler.o

Second_pass.o: Second_pass.c Var.h Declarations.h
	gcc -c -ansi -Wall -pedantic Second_pass.c -o Second_pass.o

