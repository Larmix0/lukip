CC=gcc

CFLAGS=-Wall -Wextra -Wpedantic -g -Werror

SRC_DIR=src
BIN=bin

EXE=lar

SRCS=$(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c $(SRC_DIR)/*/*/*.c)
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
