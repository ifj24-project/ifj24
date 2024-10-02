# Lets specify compiler
CC = gcc
CFLAGS = -std=gnu99 -c

# Source files.  We keep header files separate from the cpp files.
# Recall that we will compile cpp files, but we do not compile
# the header files.
HEADER = parser/parser.h scanner.h semantic/semantic.h error/error.h
CPP = parser/parser.c scanner.c  main.c semantic/semantic.c error/error.c

# Program name
PROGNAME = compiler

# Object files
OBJ = $(CPP:.c=.o)

all: $(PROGNAME)

$(PROGNAME): $(OBJ)
	$(CC) -o $(PROGNAME) $(OBJ)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -o $@ $<

# Target clean is phony, since it doesn't
# create a file called clean.  Notice that other
# targets create files.
.PHONY: clean

# We will use target clean to delete both the object files
# and the program.
clean:
	rm $(PROGNAME) $(OBJ)