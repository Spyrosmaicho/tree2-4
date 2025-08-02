# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11

# Source files
SRC = main.c Tree.c
OBJ = $(SRC:.c=.o)
HEADERS = Tree.h

# Executable name
TARGET = main

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compile .c files to .o files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJ) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean run