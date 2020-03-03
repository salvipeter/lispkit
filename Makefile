all: lispkit

CXXFLAGS=-Wall -std=c++17 -pedantic

debug: CXXFLAGS += -g -DDEBUG -DPRINT_MEMORY_INFO
debug: lispkit

release: CXXFLAGS += -O3
release: lispkit

lispkit: lispkit.o secd.o types.o
	$(CXX) -o $@ $^

secd.o: secd.cc secd.hh

types.o: types.cc types.hh

.PHONY: clean
clean:
	$(RM) *.o lispkit
