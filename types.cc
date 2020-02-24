#include "types.hh"

// for debug
#include <iostream>
extern void putexp(Data *, std::ostream &);

void dispose(Data *d) {
	std::cout << "Deleting ";
	putexp(d, std::cout);
	std::cout << "..." << std::endl;
	// if (!(d->issymbol() && dynamic_cast<Symbol*>(d)->svalue == "NIL"))
	if (d != &nil)
		delete d;
}

bool Data::issymbol() {
	return type == DataType::SYMBOL;
}

bool Data::isnumber() {
	return type == DataType::NUMBER;
}

bool Data::iscons() {
	return type == DataType::CONS;
}

Symbol nil("NIL");

