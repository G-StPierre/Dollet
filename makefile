CC = gcc
CFLAGS = -Wall -Wextra -std=c17 -g
SRC = src/main.c src/lexer.c src/parser.c src/interpreter.c src/token.c src/ast.c
OUT_DIR = build
OUT = $(OUT_DIR)/dollet

all: $(OUT)

$(OUT): $(SRC)
	@mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

run: $(OUT)
	./$(OUT)

clean:
	rm -rf $(OUT_DIR)
