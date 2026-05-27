CC = gcc
AR = ar

CFLAGS = -Wall -Wextra -pedantic -I../../include

EXEC = execute

SRCS = \
    bus.c \
    cpu.c \
    decode.c \
    main.c \
    $(wildcard $(EXEC)/*.c)

OBJS = $(SRCS:.c=.o)

TARGET = libcore.a

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC)/%.o: $(EXEC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
