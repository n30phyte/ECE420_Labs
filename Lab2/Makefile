all: client server1 server2 server3 server4

client: Code/client.c Code/timer.h Code/common.h
	g++ -std=c++11 -fpermissive -pthread -o client Code/client.c Code/timer.h Code/common.h

server1: Code/server1.cpp Code/timer.h Code/common.h Code/server.cpp Code/server.h
	g++ -std=c++11 -fpermissive -pthread -o server1 Code/server1.cpp Code/server.cpp

server2: Code/server2.cpp Code/timer.h Code/common.h
	g++ -std=c++11 -fpermissive -pthread -o server2 Code/server2.cpp Code/timer.h Code/common.h

server3: Code/server3.cpp Code/timer.h Code/common.h
	g++ -std=c++11 -fpermissive -pthread -o server3 Code/server3.cpp Code/timer.h Code/common.h

server4: Code/server4.cpp Code/timer.h Code/common.h
	g++ -std=c++11 -fpermissive -pthread -o server4 Code/server4.cpp Code/timer.h Code/common.h

clean:
	rm -f main client server1 server2 server3 server4