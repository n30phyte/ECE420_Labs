CC = gcc
CXX = g++
override CFLAGS += -std=gnu11 -O2
override CXXFLAGS += -std=gnu++11 -fpermissive -O2

.PHONY: clean

all: datatrim rename

datatrim: datatrim.c
	$(CC) $(CFLAGS) datatrim.c -o $@

rename: task1.txt task2.txt task3.txt task4.txt 

task1.txt:
	cp task1.scala task1.txt

task2.txt:
	cp task2.scala task2.txt

task3.txt:
	cp task3.scala task3.txt

task4.txt:
	cp task4.scala task4.txt

clean:
	rm -f main datatrim data_input* data_output*
