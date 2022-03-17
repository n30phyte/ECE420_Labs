CC = gcc
CXX = g++
override CFLAGS += -std=gnu11 -fopenmp -O2
override CXXFLAGS += -std=gnu++11 -fpermissive -fopenmp -O2

.PHONY: clean

all: main datagen serialtester

main: main.cpp IO.c IO.h timer.h
	$(CXX) $(CXXFLAGS) main.cpp IO.c -o $@

datatrim: datrim.c
	$(CC) $(CFLAGS) datagen.c -o $@

serialtester: serialtester.c IO.c IO.h
	$(CC) $(CFLAGS) serialtester.c IO.c -lm -o $@

clean:
	rm -f main datagen serialtester data_input data_output
