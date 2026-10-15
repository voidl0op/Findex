CC      = gcc
CFLAGS  = -Wall -Wextra -g
TARGET  = Findex

all: $(TARGET)

$(TARGET): main.c crawl.c crawl.h
	$(CC) $(CFLAGS) -o $@ main.c crawl.c

clean:
	rm -f $(TARGET)

.PHONY: all clean
