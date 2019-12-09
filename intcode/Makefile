CC=gcc
LIBS=-lm
CFLAGS=-g -Wall

DEPS=list.h intcode.h
OBJ=list.o intcode.o

all: $(OBJ)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o $(TARGET)
