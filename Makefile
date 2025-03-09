#Compiler
CC = gcc

#Flags
CCLAGS = -Wall -std=c99 -g -Werror

#The final executable name
TARGET = showFDtables

#Object files
OBJ = showFDtables.o parse_command_line.o process_info.o process_cdt.o table_display.o

HEADER = process.h parse_command_line.h table_display.h

## all: Compile all files to executable showFDtables
all: ${TARGET}

## showFDtables: Link all relevant object files if they exist
${TARGET}: ${OBJ}
	@echo "->Linking object files $< into $@..."
	${CC} ${CCLAGS} -o $@ $^
	@echo "Executable file $@ successfully created."

#Compiles all .c files to .o files, do so even if changes are made to header files 
%.o: %.c ${HEADER}
	@echo "->Compiling $< into $@..."
	${CC} ${CCLAGS} -c $<
	@echo ""

## clean: Remove all compiled files
.PHONY: clean
clean:
	@echo "Cleaning up executable and object files..."
	rm -f ${TARGET} *.o
	@echo "Finish cleaning."

## help: Displays available targets to be used.
.PHONY: help
help: Makefile
	@echo "Commands in this Makefile:"
	@sed -n 's/^##//p' $<