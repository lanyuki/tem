# Makefile
#

CC = gcc

DEBUG = -g -O0 

CFLAGS += $(DEBUG)

PROGS = ${patsubst %.c, %, ${wildcard *.c}} 

all : $(PROGS)

%:%.c
	$(CC)  $(CFLAGS) -c $< -o $@ -lpthread

.PHONY: uninstall clean dist

uninstall :
	rm -f $(PROGS) *.o

clean : uninstall
	@- $(RM) -f $(PROGS) core *.gz

	
