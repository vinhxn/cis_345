#
# Makefile for SThreads
#

CC=gcc
CFLAGS=-g -m32 -O2 -Wall
LDFLAGS=-g -m32

.c.s:
	$(CC) $(CFLAGS) $(CPPFLAGS) -S $<

libsthread.a: sthread.o sync.o
	$(AR) cr $@ $^

test: test.o libsthread.a
	$(CC) $(LDFLAGS) -o $@ test.o -L. -lsthread

clean:
	$(RM) libsthread.a test *.o *~
