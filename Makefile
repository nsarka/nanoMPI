.PHONY : clean

CC = gcc
CPPFLAGS = -g -fPIC
LDFLAGS = -shared

SOURCES = mpi.c comm.c group.c proc.c
HEADERS = mpi.h comm.h group.h proc.h status.h
OBJECTS = $(SOURCES:.c=.o)

all: libmpi.so mpirun test_hello test_pt2pt

clean:
	rm -f $(OBJECTS) libmpi.so test_mpi

libmpi.so: $(OBJECTS)
	$(CC) $(CPPFLAGS)  $(OBJECTS) -o $@ $(LDFLAGS)

mpirun: mpirun.c
	$(CC) -g -o $@ mpirun.c -L. -I.

test_hello: libmpi.so
	$(CC) -g -o tests/$@ tests/test_hello.c -L. -I. -lmpi

test_pt2pt: libmpi.so
	$(CC) -g -o tests/$@ tests/test_pt2pt.c -L. -I. -lmpi