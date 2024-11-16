
CC = gcc
CFLAGS = -std=gnu99 -c -g -Wall

HEADER = parser.h expressionparse.h scanner.h semantic.h error.h stringlib.h symstack.h symtable.h generator.h
CPP = parser.c expressionparse.c scanner.c  main.c semantic.c error.c stringlib.c symstack.c symtable.c generator.c

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

.PHONY: clean

clean:
	rm $(PROGNAME) -fr $(OBJDIR)
