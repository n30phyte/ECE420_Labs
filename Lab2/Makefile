all: client server1 server2 server3 server4

client: Code/client.c Code/timer.h Code/common.h Code/common.c
	g++ -std=c++11 -fpermissive -pthread -o client Code/client.c

server1: Code/server1.cpp Code/common.h Code/common.c Code/server.cpp Code/server.h
	g++ -std=c++11 -pthread -o server1 Code/server1.cpp Code/server.cpp Code/common.c

server2: Code/server2.cpp Code/common.h Code/common.c Code/server.cpp Code/server.h
	g++ -std=c++11 -pthread -o server2 Code/server2.cpp  Code/server.cpp Code/common.c

server3: Code/server3.cpp Code/common.h Code/common.c Code/server.cpp Code/server.h
	g++ -std=c++11 -pthread -o server3 Code/server3.cpp  Code/server.cpp Code/common.c

server4: Code/server4.cpp Code/common.h Code/common.c Code/server.cpp Code/server.h
	g++ -std=c++11 -pthread -o server4 Code/server4.cpp  Code/server.cpp Code/common.c

clean:
	rm -f main client server1 server2 server3 server4