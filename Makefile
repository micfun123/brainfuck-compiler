CC := gcc
CFLAGS := -std=c11 -Wall -Wextra
OUT := bfc

all:
	$(CC) $(CFLAGS) -o bfc bfc.c

debug: CFLAGS += -ggdb -DDEBUG
debug: all

test: CFLAGS += -DTEST
test: all

test-debug: CFLAGS += -ggdb -DDEBUG -DTEST
test-debug: all

clean:
	rm -f $(OUT)