# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g

# Target executable name
TARGET = dsh

# Find all source and header files
SRCS = $(wildcard *.c)
HDRS = $(wildcard *.h)

# Object files (compile .c to .o)
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Compile source to object files, then link to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile .c files to .o object files
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJS)

# Run tests using BATS
test:
	bats $(wildcard ./bats/*.sh)

# Run valgrind memory check
valgrind:
	echo "pwd\nexit" | valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./$(TARGET)
	echo "pwd\nexit" | valgrind --tool=helgrind --error-exitcode=1 ./$(TARGET)

.PHONY: all clean test valgrind
