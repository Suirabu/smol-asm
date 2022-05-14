CFLAGS=-Werror -Wextra
VPATH=src/
SRC=$(wildcard src/*.c)

all: bin/smol-asm

bin/:
	mkdir -p bin/

bin/smol-asm: $(SRC) | bin/
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -r bin/

.PHONY: install
install: bin/smol-asm
	cp bin/smol-asm /usr/local/bin/smol-asm
