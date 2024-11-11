CFLAGS = -Wall
DEBUG = -Wall -DDEBUG
CC = gcc
LDFLAGS = -lpcap

SRC = $(shell find . -name "*.c" ! -path "./test/prepare_pcap.c")

p2nprobe: $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

debug: $(SRC)
	$(CC) $(DEBUG) $^ -o $@ $(LDFLAGS)

test: p2nprobe
	gcc -Wall test/prepare_pcap.c -o prepare_pcap
	./prepare_pcap &
	./p2nprobe 127.0.0.1:12345 list.pcap -a 5 -i 2
	rm prepare_pcap
	cp netflow.pcap /mnt/c/Users/Maksim/Downloads/netflow.pcap

clean:
	rm -f p2nprobe debug