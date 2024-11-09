
CC = gcc
CFLAGS = -std=gnu99 -c -g 

HEADER = parser/parser.h parser/expressionparse.h scanner.h semantic/semantic.h error/error.h stringlib.h symstack.h symtable.h
CPP = parser/parser.c parser/expressionparse.c scanner.c  main.c semantic/semantic.c error/error.c stringlib.c symstack.c symtable.c

# Program name
PROGNAME = compiler

# Object files
OBJ = $(CPP:.c=.o)

all: $(PROGNAME)

$(PROGNAME): $(OBJ)
	$(CC) -o $(PROGNAME) $(OBJ) -lm

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -o $@ $< 

.PHONY: clean

clean:
	rm $(PROGNAME) $(OBJ)