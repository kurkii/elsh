CC = gcc
LD = ld
LDFLAGS = -m elf_x86_64
CFLAGS = -O2 -march=x86-64 -Wall
BUILD_DIR = target

all:
	-mkdir $(BUILD_DIR)

	$(CC) -c parser.c -o $(BUILD_DIR)/parser.o $(CFLAGS)
	$(CC) -c main.c -o $(BUILD_DIR)/main.o $(CFLAGS)
	$(CC) -c builtins.c -o $(BUILD_DIR)/builtins.o $(CFLAGS)
	$(CC) -c pipe.c -o $(BUILD_DIR)/pipe.o $(CFLAGS)

	$(CC) -o $(BUILD_DIR)/elsh $(BUILD_DIR)/parser.o $(BUILD_DIR)/main.o $(BUILD_DIR)/builtins.o $(BUILD_DIR)/pipe.o 

	chmod +x $(BUILD_DIR)/elsh