CC=gcc
SRCDIR=src
INCDIR=$(SRCDIR)
TESTDIR=tests

CFLAGS=-Wall -Wextra -Werror -Wpedantic -pedantic-errors
LOPT=-lglfw

HEADERS=$(INCDIR)/mcapt.h $(INCDIR)/mlog.h
OBJECTS=$(SRCDIR)/mcapt.o

all:	mcapt test

mcapt:	$(OBJECTS) $(SRCDIR)/main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LOPT)

test:	$(OBJECTS) $(TESTDIR)/test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LOPT)

%.o:	%.c $(HEADERS)
	$(CC) $(COPT)-c $< -o $@ -I$(INCDIR)

clean:
	rm -f $(SRCDIR)/*.o $(TESTDIR)/*.o mcapt test
