all: main matrixgen serialtester

main: Lab1/Code/main.cpp Lab1/Code/IO.c Lab1/Code/IO.h
	g++ -fpermissive -pthread -o main Lab1/Code/main.cpp Lab1/Code/IO.c

matrixgen: Lab1/Code/matrixgen.c
	g++ -o matrixgen Lab1/Code/matrixgen.c

serialtester: Lab1/Code/serialtester.c Lab1/Code/IO.c Lab1/Code/IO.h
	g++ -fpermissive -o serialtester Lab1/Code/serialtester.c Lab1/Code/IO.c

clean:
	rm -f main matrixgen serialtester data_input data_output