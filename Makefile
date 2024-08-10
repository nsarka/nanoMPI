.PHONY : clean

CC = g++
CPPFLAGS = -g -fPIC
LDFLAGS = -shared

SOURCES = mpi.cpp
HEADERS = mpi.h
OBJECTS = $(SOURCES:.cpp=.o)

all: libmpi.so test_mpi

clean:
	rm -f $(OBJECTS) libmpi.so test_mpi

libmpi.so: $(OBJECTS)
	$(CC) $(CPPFLAGS)  $(OBJECTS) -o $@ $(LDFLAGS)

test_mpi: libmpi.so
	$(CC) -g -o $@ test_mpi.cpp -L. -lmpi