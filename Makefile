all: secd

CXXFLAGS=-Wall -g -std=c++17 -pedantic

secd: secd.o reader.o types.o
	g++ -o $@ $^

reader.o: reader.cc reader.hh

types.o: types.cc types.hh

.PHONY: clean
clean:
	rm *.o secd
