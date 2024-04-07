CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g -Werror -Iinclude

SRC_DIR = src
TEST_DIR = tests

BIN = bin
EXE = lukip

SRCS := $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c $(SRC_DIR)/*/*/*.c)
TESTS := $(wildcard $(TEST_DIR)/*.c $(TEST_DIR)/*/*.c $(TEST_DIR)/*/*/*.c)

ifeq ($(OS), Windows_NT)
	EXE = lukip.exe
	SRCS := $(subst /,\,$(SRCS))
	TESTS := $(subst /,\,$(TESTS))
endif

OBJS = $(SRCS:.c=.o)
TEST_OBJS = $(TESTS:.c=.o)

# "newline" resolves to an actual escape "\n" sequence (hence endef is an extra line down).
define newline


endef

.PHONY: all clean lib tests

all: lib

lib: $(OBJS)
	ar rcs liblukip.a $^

tests: $(EXE)

$(EXE): $(BIN) $(OBJS) $(TEST_OBJS)
ifeq ($(OS), Windows_NT)
	$(CC) -o $<\$@ $(OBJS) $(TEST_OBJS)
else
	$(CC) -o $</$@ $(OBJS) $(TEST_OBJS)
endif

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(BIN):
	mkdir $(BIN)

clean:
ifeq ($(OS), Windows_NT)
	$(foreach obj, $(OBJS) $(TEST_OBJS), if exist $(obj) del /s /q $(obj) > NUL$(newline))
	if exist $(BIN) rmdir /s /q $(BIN)
	if exist liblukip.a del /s /q liblukip.a > NUL
else
	rm -rf $(OBJS) $(TEST_OBJS) $(BIN) liblukip.a
endif
