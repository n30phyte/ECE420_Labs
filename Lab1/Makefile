all: main matrixgen serialtester

main: Code/main.cpp Code/IO.c Code/IO.h Code/timer.h
	g++ -std=c++11 -fpermissive -pthread -o main Code/main.cpp Code/IO.c

matrixgen: Code/matrixgen.c
	g++ -std=c++11 -fpermissive -o matrixgen Code/matrixgen.c

serialtester: Code/serialtester.c Code/IO.c Code/IO.h
	g++ -std=c++11 -fpermissive -o serialtester Code/serialtester.c Code/IO.c

clean:
	rm -f main matrixgen serialtester data_input data_output