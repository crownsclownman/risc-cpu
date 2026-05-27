CC = gcc
AR = ar

CFLAGS = -Wall -Wextra -pedantic \
         -I../../include

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

TARGET = libdevices.a

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
