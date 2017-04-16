CC      = g++
MPICC 	= mpic++ 
CFLAGS  = -std=c++11 -Wall -O2 
LDFLAGS = -lmpi 

all: timeAdderSerial timeAdder

timeAdderSerial: timeAdderSerial.o
		$(CC) -o $@ $^ $(LDFLAGS)

timeAdderSerial.o: timeAdderSerial.cpp
		$(CC) -c $(CFLAGS) $<

timeAdder: timeAdder.o
		$(MPICC) -o $@ $^ $(LDFLAGS)

timeAdder.o: timeAdder.cpp
		$(MPICC) -c $(CFLAGS) $<

.PHONY: clean

clean:
		rm *.o timeAdderSerial timeAdder

# $@ name of target
# $^ name of prerequisites with duplicates removed
# $< name of first prerequisite
