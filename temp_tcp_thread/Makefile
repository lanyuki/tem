# Makefile
#

CC = gcc

DEBUG = -g -O0 

CFLAGS += $(DEBUG)

PROGS = ${patsubst %.c, %, ${wildcard *.c}} 

all : $(PROGS)

%:%.c
	$(CC)  $(CFLAGS) $< -o $@ -lpthread -ljson -lsqlite3

.PHONY: uninstall clean dist

uninstall :
	rm -f $(PROGS) *.o *.db

clean : uninstall
	@- $(RM) -f $(PROGS) core *.gz

	
