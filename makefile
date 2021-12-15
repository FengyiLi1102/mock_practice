main: main.o sonnet.o
	g++ -Wall -g sonnet.o main.o -o main

main.o: main.cpp
	g++ -Wall -c -g main.cpp

sonnet.o: sonnet.cpp sonnet.h
	g++ -Wall -g -c sonnet.cpp

clean:
	rm -rf *.o main