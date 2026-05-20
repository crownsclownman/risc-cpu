CC = gcc

CFLAGS = -Wall \
          -Wextra \
          -pedantic \
          -Iinclude

LDFLAGS =

TARGET = emu

SRC_DIR = src

SRCS = \
    $(wildcard $(SRC_DIR)/core/*.c) \
    $(wildcard $(SRC_DIR)/core/execute/*.c) \
    $(wildcard $(SRC_DIR)/machine/*.c) \
    $(wildcard $(SRC_DIR)/devices/*.c)

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)

rebuild: clean all

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run
