all: client server1 server2 server3 server4

client: client.c timer.h common.h common.c
	g++ -std=c++11 -fpermissive -pthread -o client client.c

server1: server1.cpp common.h common.c server.cpp server.h
	g++ -std=c++11 -pthread -o server1 server1.cpp server.cpp common.c

server2: server2.cpp common.h common.c server.cpp server.h
	g++ -std=c++11 -pthread -o server2 server2.cpp  server.cpp common.c

server3: server3.cpp common.h common.c server.cpp server.h
	g++ -std=c++11 -pthread -o server3 server3.cpp  server.cpp common.c

server4: server4.cpp common.h common.c server.cpp server.h
	g++ -std=c++11 -pthread -o server4 server4.cpp  server.cpp common.c

clean:
	rm -f main client server1 server2 server3 server4