CC      = g++
MPICC 	= mpic++ 
CFLAGS  = -std=c++11 -Wall -O2 
LDFLAGS = -lmpi 

all: gameOfLifeSerial gameOfLife

gameOfLifeSerial: gameOfLifeSerial.o
		$(CC) -o $@ $^ $(LDFLAGS)

gameOfLifeSerial.o: gameOfLifeSerial.cpp
		$(CC) -c $(CFLAGS) $<

gameOfLife: gameOfLife.o
		$(MPICC) -o $@ $^ $(LDFLAGS)

gameOfLife.o: gameOfLife.cpp
		$(MPICC) -c $(CFLAGS) $<

.PHONY: clean

clean:
		rm *.o gameOfLifeSerial gameOfLife

# $@ name of target
# $^ name of prerequisites with duplicates removed
# $< name of first prerequisite
