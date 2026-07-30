SOURCES = $(wildcard *.c)
TARGET = $(notdir $(CURDIR)).out
CC = gcc
CFLAGS = -Wall -Wextra -O2

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

.PHONY: all clean
