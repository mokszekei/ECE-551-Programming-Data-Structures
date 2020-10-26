CFLAGS=-std=c++11 -DWITHOUT_NUMPY -I/usr/include/python3.6m -lpython3.6m


mesh: main2.o
	g++ $(CFLAGS) -o mesh main2.o
main2.o: mesh.cpp mesh.h
	g++ $(CFLAGS) -o main2.o -c mesh.cpp
