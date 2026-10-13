CC      = gcc
CFLAGS  = -Wall -Wextra -g
TARGET  = FileBrowser

all: $(TARGET)

$(TARGET): main.c crawl.c crawl.h
	$(CC) $(CFLAGS) -o $@ main.c file.c

clean:
	rm -f $(TARGET)

.PHONY: all clean