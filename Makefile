# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -std=c99 -g -Werror

# Executable name
TARGET = showFDtables

# Source files
SRC = showFDtables.c parse_command_line.c process_info.c process_cdt.c table_display.c

# Object files (same as source files but with .o)
OBJ = $(SRC:.c=.o)

HEADER = process.h parse_command_line.h table_display.h

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c ${HEADER}
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJ)
