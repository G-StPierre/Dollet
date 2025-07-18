CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
SRC = main.c lexer.c parser.c interpreter.c token.c ast.c
OUT_DIR = build
OUT = $(OUT_DIR)/dollet

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
