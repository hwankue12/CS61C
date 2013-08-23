CC=gcc
CFLAGS=-fopenmp -O3 -g -std=gnu99
LDFLAGS=-fopenmp

all: v_add hello dotp

v_add: v_add.o

hello: hello.o

dotp: dotp.o

clean:
	rm -rf hello.o hello v_add.o v_add dotp.o dotp
