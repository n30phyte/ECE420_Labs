CC = gcc
CXX = g++
override CFLAGS += -std=c11 -fopenmp -O2
override CXXFLAGS += -std=c++11 -fpermissive -fopenmp -O2

.PHONY: clean

main: main.cpp IO.c IO.h timer.h
	$(CXX) $(CXXFLAGS) main.cpp IO.c -o $@

datagen: datagen.c IO.c IO.h
	$(CC) $(CFLAGS) datagen.c IO.c -o $@

serialtester: datagen.c IO.c IO.h
	$(CC) $(CFLAGS) serialtester.c IO.c -o $@

clean:
	rm -f main datagen serialtester
