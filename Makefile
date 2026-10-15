CC      = gcc
CFLAGS  = -Wall -Wextra -g
TARGET  = Findex

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $@ main.c

clean:
	rm -f $(TARGET)

.PHONY: all clean
