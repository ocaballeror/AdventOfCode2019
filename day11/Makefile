IDIR=../intcode
CC=gcc
LIBS=-lm
CFLAGS=-g -Wall -I$(IDIR)

DEPS=../intcode/*.o
TARGET=sol1

all: sol1

$(DEPS):
	make -C ../intcode

%.o: %.c
	$(CC) -c -o $@ $^ $(CFLAGS)

sol1: sol1.o dequeue.o $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o $(TARGET)
