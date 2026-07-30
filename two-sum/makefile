SOURCES = $(wildcard *.c)
TARGETS = $(SOURCES:.c=.out)
CC = gcc
CFLAGS = -Wall -Wextra -O2

all: $(TARGETS)

%.out: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

.PHONY: all clean
