
CC = gcc
CFLAGS = -std=gnu99 -c -g -Wall

HEADER = parser/parser.h parser/expressionparse.h scanner.h semantic/semantic.h error/error.h stringlib.h symstack.h symtable.h generator.h
CPP = parser/parser.c parser/expressionparse.c scanner.c  main.c semantic/semantic.c error/error.c stringlib.c symstack.c symtable.c generator.c

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
	@mkdir -p $(OBJDIR)/parser
	@mkdir -p $(OBJDIR)/error
	@mkdir -p $(OBJDIR)/semantic
	$(CC) $(CFLAGS) -o $@ $< 

.PHONY: clean

clean:
	rm $(PROGNAME) -fr $(OBJDIR)