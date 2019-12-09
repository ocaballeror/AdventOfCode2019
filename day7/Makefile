IDIR=../intcode
CC=gcc
LIBS=-lm
CFLAGS=-g -Wall -I$(IDIR)

DEPS=../intcode/*.o
OBJ=sol1.o sol2.o
TARGET=sol1 sol2

all: sol1 sol2

$(DEPS):
	make -C ../intcode

%.o: %.c
	$(CC) -c -o $@ $^ $(CFLAGS)

test: test.o $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

sol1: sol1.o $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

sol2: sol2.o $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o $(TARGET)
