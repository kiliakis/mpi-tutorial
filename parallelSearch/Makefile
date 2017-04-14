CC      = g++
MPICC 	= mpic++ 
CFLAGS  = -std=c++11 -Wall -O2 
LDFLAGS = -lmpi

all: parallelSearchSerial parallelSearch 

parallelSearchSerial: parallelSearchSerial.o
	$(CC) -o $@ $^ $(LDFLAGS)

parallelSearchSerial.o: parallelSearchSerial.cpp
	$(CC) -c $(CFLAGS) $<

parallelSearch: parallelSearch.o
	$(MPICC) -o $@ $^ $(LDFLAGS)

parallelSearch.o: parallelSearch.cpp
	$(MPICC) -c $(CFLAGS) $<

.PHONY: clean

clean:
	rm *.o parallelSearchSerial parallelSearch
		
# $@ name of target
# $^ name of prerequisites with duplicates removed
# $< name of first prerequisite
