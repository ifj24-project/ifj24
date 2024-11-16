
CC = gcc
CFLAGS = -std=gnu99 -c -g -Wall

HEADER = parser.h expressionparse.h scanner.h semantic.h error.h stringlib.h symtable.h generator.h
CPP = parser.c expressionparse.c scanner.c  main.c semantic.c error.c stringlib.c symtable.c generator.c

FILES_TO_PACK = $(HEADER) $(CPP) Makefile
PACK_NAME = xgeierd00

# Program name
PROGNAME = compiler

# Object files
OBJDIR = bin
OBJ = $(addprefix $(OBJDIR)/, $(CPP:.c=.o))

all: $(PROGNAME)

$(PROGNAME): $(OBJ)
	$(CC) -o $(PROGNAME) $(OBJ) -lm

$(OBJDIR)/%.o: %.c $(HEADER)
	@mkdir -p $(OBJDIR)/
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean pack

clean:
	rm $(PROGNAME) -fr $(OBJDIR)

pack:
	tar -zcvf $(PACK_NAME).tar.gz $(FILES_TO_PACK)
