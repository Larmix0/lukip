CC=gcc

CFLAGS=-Wall -Wextra -Wpedantic -g -Werror

SRCDIR=src
TESTDIR=tests
BIN=bin

EXE=lar

SRCS=$(wildcard $(SRCDIR)/*.c $(SRCDIR)/*/*.c $(SRCDIR)/*/*/*.c $(TESTDIR)/*.c $(TESTDIR)/*/*.c)
OBJS=$(SRCS:.c=.o)

.PHONY: all clean

all: $(EXE)

$(EXE): $(BIN) $(OBJS)
	$(CC) -o $</$@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(BIN):
	mkdir $(BIN)

clean:
	rm -rf $(OBJS) $(BIN)
