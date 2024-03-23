CC=gcc
SRCDIR=.
INCDIR=.

CFLAGS=-Wall -Wextra -Werror -Wpedantic -pedantic-errors
LOPT=-lSDL2

HEADERS=$(INCDIR)/mcapt.h $(INCDIR)/mscreen.h $(INCDIR)/mlog.h
OBJECTS=mcapt.o mscreen.o

all:	mcapt test

mcapt:	$(OBJECTS) main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LOPT)

test:	$(OBJECTS) test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LOPT)

%.o:	%.c $(HEADERS)
	$(CC) $(COPT) -c $< -o $@

clean:
	rm -f $(SRCDIR)/*.o mcapt test
