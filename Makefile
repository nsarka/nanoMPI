.PHONY : clean

CC = gcc
CXX = g++
CPPFLAGS = -g -fPIC -O0 -I.
LDFLAGS = -shared

SOURCES_C = mpi.c comm.c group.c proc.c op.c dtype.c util.c socket_backend.c colls/tuner.c colls/allgather/allgather.c colls/allgatherv/allgatherv.c colls/allreduce/allreduce.c colls/alltoall/alltoall.c colls/bcast/bcast.c colls/gather/gather.c colls/reduce/reduce.c colls/reduce_scatter/reduce_scatter.c colls/scatter/scatter.c
SOURCES_CPP = self_backend.cpp
OBJECTS_CPP = $(SOURCES_CPP:.cpp=.o)
OBJECTS_C = $(SOURCES_C:.c=.o) 

all: libmpi.so mpirun tests

clean:
	rm -f $(OBJECTS_C) $(OBJECTS_CPP) libmpi.so 

libmpi.so: $(OBJECTS_C) $(OBJECTS_CPP)
	$(CXX) $(CPPFLAGS) $(OBJECTS_C) $(OBJECTS_CPP) -o $@ $(LDFLAGS)

mpirun: mpirun.c libmpi.so
	$(CC) -g -o $@ mpirun.c -L. -I. -lpthread

tests: test_hello test_pt2pt test_bcast test_reduce test_allreduce test_scatter_gather test_alltoall test_self

test_hello: libmpi.so
	$(CC) -g -o tests/$@ tests/test_hello.c -L. -I. -lmpi

test_pt2pt: libmpi.so
	$(CC) -g -o tests/$@ tests/test_pt2pt.c -L. -I. -lmpi

test_bcast: libmpi.so
	$(CC) -g -o tests/$@ tests/test_bcast.c -L. -I. -lmpi

test_reduce: libmpi.so
	$(CC) -g -o tests/$@ tests/test_reduce.c -L. -I. -lmpi

test_allreduce: libmpi.so
	$(CC) -g -o tests/$@ tests/test_allreduce.c -L. -I. -lmpi

test_scatter_gather: libmpi.so
	$(CC) -g -o tests/$@ tests/test_scatter_gather.c -L. -I. -lmpi

test_alltoall: libmpi.so
	$(CC) -g -o tests/$@ tests/test_alltoall.c -L. -I. -lmpi

test_self: libmpi.so
	$(CC) -g -o tests/$@ tests/test_self.c -L. -I. -lmpi
