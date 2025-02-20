# Compiler and flags
CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -g

TARGET  = sdbsc

SRCS    = sdbsc.c
OBJS    = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c db.h sdbsc.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
