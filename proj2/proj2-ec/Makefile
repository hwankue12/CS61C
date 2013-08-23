HOME = /home/ff/cs61c
UNAME = $(shell uname)

# running on hive machines
ifeq ($(UNAME),Linux)
CC = gcc -std=gnu99
GOTO = $(HOME)/bin/GotoBLAS2_Linux
PTHREAD = /usr/lib/libgpgme-pthread.so.11
GOTOLIB = $(GOTO)/libgoto2_nehalemp-r1.13.a
endif

# running on 200 SD machines
ifeq ($(UNAME),Darwin)
CC = gcc -std=gnu99
GOTO = $(HOME)/bin/GotoBLAS2
GOTOLIB = $(GOTO)/libgoto2_nehalemp-r1.13.a
endif

INCLUDES = -I$(GOTO) -I/usr/lib
OMP = -fopenmp
LIBS = -lpthread 
# a pretty good flag selection for this machine...
CFLAGS = -msse4 -fopenmp -O3 -pipe -fno-omit-frame-pointer

all:	bench-naive bench-threads bench-test

# triple nested loop implementation
bench-naive: benchmark.o sgemm-naive.o
	$(CC) -o $@ $(LIBS) benchmark.o sgemm-naive.o $(GOTOLIB)

# your implementation for extra credit
bench-threads: benchmark.o sgemm-threads.o
	$(CC) -o $@ $(LIBS) $(OMP) benchmark.o sgemm-threads.o $(GOTOLIB)

bench-test: tester.o sgemm-threads.o
	$(CC) -o $@ $(LIBS) $(OMP) tester.o sgemm-threads.o $(GOTOLIB)

%.o: %.c
	$(CC) -c -pthread $(CFLAGS) $(INCLUDES) $<

clean:
	rm -f *~ bench-naive bench-threads bench-test core *.o
