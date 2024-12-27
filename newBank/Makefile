# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Iinclude -g

# Source and object files
SRC_SERVER = src/server.c src/bank.c
SRC_CLIENT = src/client.c src/bank.c

# Object files
OBJ_SERVER = $(SRC_SERVER:.c=.o)
OBJ_CLIENT = $(SRC_CLIENT:.c=.o)

# Output executable name
OUTPUT_SERVER = server
OUTPUT_CLIENT = client

# Default target to build the program
all: $(OUTPUT_SERVER) $(OUTPUT_CLIENT)

# Target to build server
server: $(OUTPUT_SERVER)

# Target to build client
client: $(OUTPUT_CLIENT)

# Rule to link object files into executable for server
$(OUTPUT_SERVER): $(OBJ_SERVER)
	$(CC) $(OBJ_SERVER) -o $(OUTPUT_SERVER)

# Rule to link object files into executable for client
$(OUTPUT_CLIENT): $(OBJ_CLIENT)
	$(CC) $(OBJ_CLIENT) -o $(OUTPUT_CLIENT)

# Rule to compile each .c file to a .o file
src/%.o: src/%.c include/bank.h
	$(CC) $(CFLAGS) -c $< -o $@

# clean target to remove compiled files
clean:
	rm -rf $(OBJ_CLIENT) $(OBJ_SERVER) $(OUTPUT_CLIENT) $(OUTPUT_SERVER)
