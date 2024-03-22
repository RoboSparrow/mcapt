CC=gcc
SRCDIR=.
INCDIR=.

CFLAGS=-Wall -Wextra -Werror -Wpedantic -pedantic-errors
CFLAGS=

HEADERS=$(INCDIR)/mcapt.h
OBJECTS=mcapt.o

all:	mcapt test

mcapt:	$(OBJECTS) main.o
	$(CC) $(CFLAGS) -o $@ $^

test:	$(OBJECTS) test.o
	$(CC) $(CFLAGS) -o $@ $^

%.o:	%.c $(HEADERS)
	$(CC) $(COPT) -c $< -o $@

clean:
	rm -f $(SRCDIR)/*.o mcapt test
