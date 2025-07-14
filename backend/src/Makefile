# Makefile for codestat-runall (headless version only)

CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SRC = src/main.c \
      src/scanner.c \
      src/analyzer/analyzer.c \
      src/analyzer/loc.c \
      src/analyzer/complexity.c \
      src/analyzer/function_count.c \
      src/analyzer/comment_todo.c

TARGET = codestat-runall

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) temp_func.txt report.json
