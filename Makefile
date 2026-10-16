CC      = gcc
CFLAGS  = -Wall -Wextra -g
TARGET  = Findex

all: $(TARGET)

$(TARGET): main.c file.c file.h
	$(CC) $(CFLAGS) -o $@ main.c file.c

clean:
	rm -f $(TARGET)

.PHONY: all clean
