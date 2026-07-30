TARGET = $(notdir $(CURDIR)).out
SOURCE = source.c
CC = gcc
CFLAGS = -Wall -Wextra -O2

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

.PHONY: all clean
