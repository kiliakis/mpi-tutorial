CC      = g++
MPICC 	= mpic++ 
CFLAGS  = -std=c++11 -Wall -O2 
LDFLAGS = 

all: gameOfLifeSerial 

gameOfLifeSerial: gameOfLifeSerial.o
		$(CC) -o $@ $^ $(LDFLAGS)

gameOfLifeSerial.o: gameOfLifeSerial.cpp
		$(CC) -c $(CFLAGS) $<

.PHONY: clean cleanest

clean:
		rm *.o

cleanest: clean
		rm gameOfLifeSerial

# $@ name of target
# $^ name of prerequisites with duplicates removed
# $< name of first prerequisite
