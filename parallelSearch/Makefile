CC      = g++
MPICC 	= mpic++ 
CFLAGS  = -std=c++11 -Wall -O2 
LDFLAGS = 
:
all: parallelSearchSerial 

parallelSearchSerial: parallelSearchSerial.o
	$(CC) -o $@ $^ $(LDFLAGS)

parallelSearchSerial.o: parallelSearchSerial.cpp
	$(CC) -c $(CFLAGS) $<

.PHONY: clean

clean:
	rm *.o parallelSearchSerial
		
# $@ name of target
# $^ name of prerequisites with duplicates removed
# $< name of first prerequisite
