CC = gcc

CFLAGS = -Iinclude

SRC = src/test_main.c src/bit_utils.c

OUT = build/test_app

all:
	$(CC) $(SRC) $(CFLAGS) -o $(OUT)

clean:
	rm -f $(OUT)

run: all
	./$(OUT)