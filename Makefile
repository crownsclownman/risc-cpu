CC = gcc

CFLAGS = -Wall -Wextra -pedantic -Iinclude

TARGET = emu

LIBS = \
    src/core/libcore.a \
    src/machine/libmachine.a \
    src/devices/libdevices.a \
#    src/runtime/libruntime.a

all:
	$(MAKE) -C src
	$(CC) $(LIBS) -o $(TARGET)

clean:
	$(MAKE) -C src clean
	rm -f $(TARGET)

.PHONY: all clean
