C = gcc
CXX = g++
override CFLAGS += -std=c11 -pthread -O2
override CXXFLAGS += -std=c++11 -fpermissive -pthread -Wno-unused-result -O2

.PHONY: clean

all: client main1 main2 main3 main4 attacker

client: client.c timer.h common.h common.c
	$(C) $(CFLAGS) client.c -o $@

main1: main1.cpp common.h common.c server.cpp server.h
	$(CXX) $(CXXFLAGS) main1.cpp server.cpp common.c -o $@

main2: main2.cpp common.h common.c server.cpp server.h
	$(CXX) $(CXXFLAGS) main2.cpp server.cpp common.c -o $@

main3: main3.cpp common.h common.c server.cpp server.h
	$(CXX) $(CXXFLAGS) main3.cpp server.cpp common.c -o $@

main4: main4.cpp common.h common.c server.cpp server.h
	$(CXX) $(CXXFLAGS) main4.cpp server.cpp common.c -o $@

attacker: attacker.c common.c common.h
	$(C) $(CFLAGS) attacker.c -lm -o $@

clean:
	rm -f client main1 main2 main3 main4 attacker
