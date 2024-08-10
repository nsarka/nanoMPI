.PHONY : clean

CC = gcc
CPPFLAGS = -g -fPIC
LDFLAGS = -shared

SOURCES = mpi.c comm.c group.c proc.c
HEADERS = mpi.h comm.h group.h proc.h
OBJECTS = $(SOURCES:.c=.o)

all: libmpi.so test_mpi

clean:
	rm -f $(OBJECTS) libmpi.so test_mpi

libmpi.so: $(OBJECTS)
	$(CC) $(CPPFLAGS)  $(OBJECTS) -o $@ $(LDFLAGS)

test_mpi: libmpi.so
	$(CC) -g -o $@ test_mpi.c -L. -lmpi