IDIR=../intcode
CC=gcc
LIBS=-lm
CFLAGS=-g -Wall -I$(IDIR)

DEPS=../intcode/*.o
OBJ=day9.o
TARGET=day9

all: day9

$(DEPS):
	make -C ../intcode

%.o: %.c
	$(CC) -c -o $@ $^ $(CFLAGS)

day9: day9.o $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o $(TARGET)
