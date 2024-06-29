#
# Makefile for the UM
# 
CC = gcc

CFLAGS  = -g -std=gnu99 -Wall -Wextra -Werror -pedantic

# Libraries to link against
LIBS = -lreadline


# Collect all .h files in your directory.
# This way, you can never forget to add
# a local .h file in your dependencies.
#
# This bugs Mark, who dislikes false dependencies, but
# he agrees with Noah that you'll probably spend hours 
# debugging if you forget to put .h files in your 
# dependency list.
INCLUDES = $(shell echo *.h)

EXECS   = MASH

all: $(EXECS)

MASH: MASH.o get_num.o init.o builtin.o mash_process.o
	$(CC) $^ -o $@ $(LIBS)

# tests: tests.o memory.o
# 	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECS)  *.o
