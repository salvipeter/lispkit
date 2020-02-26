#include "types.hh"

void dispose(Data *d) {
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

