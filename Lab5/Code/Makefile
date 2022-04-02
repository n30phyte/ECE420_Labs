CC = gcc
CXX = g++
override CFLAGS += -std=gnu11 -O2
override CXXFLAGS += -std=gnu++11 -fpermissive -O2

.PHONY: clean

all: datatrim

datatrim: datatrim.c
	$(CC) $(CFLAGS) datatrim.c -o $@

clean:
	rm -f main datatrim data_input* data_output*
