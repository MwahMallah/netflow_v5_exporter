CFLAGS = -Wall
DEBUG = -Wall -DDEBUG
CC = gcc
LDFLAGS = -lpcap

SRC = $(shell find . -name "*.c")

p2nprobe: $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

debug: $(SRC)
	$(CC) $(DEBUG) $^ -o $@ $(LDFLAGS)

clean:
	rm -f p2nprobe debug