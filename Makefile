# Makefile: Copyright Grigore Mihnea-Andrei
CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lm
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=)

build: clean_nooutput $(OBJECTS)

%: %.c
	@$(CC) $(CFLAGS) -g -o $@ $< $(LDFLAGS)

run: ./tema1

clean:
	rm -f $(OBJECTS)

clean_nooutput:
	@rm -f $(OBJECTS) > /dev/null

copyright:
	@echo "(Copyright): Grigore Mihnea-Andrei"