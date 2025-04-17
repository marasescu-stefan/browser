CC=gcc
CFLAGS=-Wall -Wextra -std=c99

TARGETS=tema1

build: $(TARGETS)

tema1: browser.c
	$(CC) $(CFLAGS) *.c -o tema1 -lm

run: tema1
	./$(TARGETS)

pack:
	zip -FSr 311CA_MarasescuStefan_Tema1.zip Makefile README *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
