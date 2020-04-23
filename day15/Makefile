IDIR=../intcode
CC=gcc
LIBS=-lm
CFLAGS=-g -Wall -I$(IDIR)

DEPS=../intcode/*.o
TARGET=day15

all: day15

$(DEPS):
	make -C ../intcode

%.o: %.c
	$(CC) -c -o $@ $^ $(CFLAGS)

day15: day15.o $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o $(TARGET)
