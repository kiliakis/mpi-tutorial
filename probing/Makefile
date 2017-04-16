CC      = g++
MPICC 	= mpic++ 
CFLAGS  = -std=c++11 -Wall -O2 
LDFLAGS = -lmpi 

all: probing

probing: probing.o
		$(MPICC) -o $@ $^ $(LDFLAGS)

probing.o: probing.cpp
		$(MPICC) -c $(CFLAGS) $<

.PHONY: clean

clean:
		rm *.o probing

# $@ name of target
# $^ name of prerequisites with duplicates removed
# $< name of first prerequisite
