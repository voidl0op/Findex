CC      = gcc
CFLAGS  = -Wall -Wextra -g -Iinclude $(shell pkg-config --cflags gtk4)
LDLIBS  = $(shell pkg-config --libs gtk4)
TARGET  = Findex

SRC     = $(wildcard src/*.c)
HDR     = $(wildcard include/*.h)

all: $(TARGET)

$(TARGET): $(SRC) $(HDR)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(LDLIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean