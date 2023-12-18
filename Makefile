CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -g -Werror

OS = $(shell uname)

SRCDIR = src
TESTDIR = tests
BIN = bin

ifeq ($(OS), Darwin)
	EXE = lukip.exe
else
	EXE = lukip
endif

SRCS=$(wildcard $(SRCDIR)/*.c $(SRCDIR)/*/*.c $(SRCDIR)/*/*/*.c)
TESTS=$(wildcard $(TESTDIR)/*.c $(TESTDIR)/*/*.c $(TESTDIR)/*/*/*.c)
OBJS=$(SRCS:.c=.o)
TESTOBJS=$(TESTS:.c=.o)

.PHONY: all clean

all: $(EXE)

$(EXE): $(BIN) $(OBJS) $(TESTOBJS)
	$(CC) -o $</$@ $(OBJS) $(TESTOBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(BIN):
	mkdir $(BIN)

clean:
	rm -rf $(OBJS) $(TESTOBJS) $(BIN)
