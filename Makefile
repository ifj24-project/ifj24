
CC = gcc
CFLAGS = -std=gnu99 -c -g -Wall

HEADER = parser.h expressionparse.h scanner.h semantic.h error.h stringlib.h symtable.h generator.h
CPP = parser.c expressionparse.c scanner.c  main.c semantic.c error.c stringlib.c symtable.c generator.c

FILES_TO_PACK = $(HEADER) $(CPP) Makefile dokumentace.pdf rozdeleni rozsireni
PACK_NAME = xgeierd00

# Program name
PROGNAME = compiler

# Object files
OBJ = $(CPP:.c=.o)

all: $(PROGNAME)

$(PROGNAME): $(OBJ)
	$(CC) -o $(PROGNAME) $(OBJ) -lm

$%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean pack clean-pack

clean:
	rm $(PROGNAME) $(OBJ)

clean-pack:
	rm $(PACK_NAME).tar.gz

pack:
	tar -zcvf $(PACK_NAME).tar.gz $(FILES_TO_PACK)
