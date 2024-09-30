#
# Makefile for SThreads
#

CC=gcc
RM=rm

all: test

test: sthread.c sync.c test.c
	$(CC) -o test sthread.c sync.c test.c

clean:
	$(RM) sthread test *.o *~
