all: lispkit

CXXFLAGS=-Wall -g -std=c++17 -pedantic

lispkit: lispkit.o secd.o types.o
	g++ -o $@ $^

secd.o: secd.cc secd.hh

types.o: types.cc types.hh

.PHONY: clean
clean:
	rm *.o lispkit
