all: lispkit

CXXFLAGS=-Wall -g -std=c++17 -pedantic

lispkit: lispkit.o secd.o reader.o types.o
	g++ -o $@ $^

secd.o: secd.cc secd.hh

reader.o: reader.cc reader.hh

types.o: types.cc types.hh

.PHONY: clean
clean:
	rm *.o lispkit
