#
# Makefile for Project3
#

CC=gcc
RM=rm

all: mempart1 mempart2

mempart1: mempart1.c
	$(CC) -o mempart1 mempart1.c

mempart2: mempart2.c
	$(CC) -o mempart2 pagetable.c phyframe.c mempart2.c

clean:
	$(RM) mempart1 mempart2 *.o *~
