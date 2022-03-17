CC = gcc
CXX = g++
MPICC = mpicc
MPICXX = mpicxx
override CFLAGS += -std=gnu11 -fopenmp -O2
override CXXFLAGS += -std=gnu++11 -fpermissive -fopenmp -O2

.PHONY: clean

all: main datatrim serialtester

main: main.cpp IO.c IO.h timer.h
	$(MPICXX) $(CXXFLAGS) main.cpp IO.c -o $@

datatrim: datatrim.c
	$(CC) $(CFLAGS) datatrim.c -o $@

serialtester: serialtester.c IO.c IO.h
	$(CC) $(CFLAGS) serialtester.c -lm -o $@

clean:
	rm -f main datatrim serialtester data_input data_output
