CFLAGS=-Wall -Werror --std=gnu++11 -pedantic -ggdb3

todo_list: main.o parse.o
	g++ $(CFLAGS) -o todo_list main.o parse.o
main.o: todo_list.cpp
	g++ $(CFLAGS) -o main.o -c todo_list.cpp
parse.o: parse.cpp
	g++ -o parse.o -c parse.cpp

