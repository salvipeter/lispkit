#include "types.hh"

Data::Data(size_t s) : type(DataType::SYMBOL), str_index(s) {
}

Data::Data(Data *a, Data *b) : type(DataType::CONS), cadr({ a, b }) {
}

Data::~Data() {
}

void Data::setSymbol(size_t s) {
  type = DataType::SYMBOL;
  str_index = s;
}

size_t Data::svalue() const {
  return str_index;
}

void Data::setNumber(int n) {
  type = DataType::NUMBER;
  number = n;
}

int Data::ivalue() const {
  return number;
}

void Data::setCons(Data *a, Data *b) {
  type = DataType::CONS;
  cadr = { a, b };
}

Data *Data::car() const {
  return cadr.first;
}

Data *Data::cdr() const {
  return cadr.second;
}

void Data::rplaca(Data *d) {
  cadr.first = d;
}

bool Data::issymbol() const {
  return type == DataType::SYMBOL;
}

bool Data::isnumber() const {
  return type == DataType::NUMBER;
}

bool Data::iscons() const {
  return type == DataType::CONS;
}

void Data::mark() {
  if (marked)
    return;
  marked = true;
  if (iscons()) {
    car()->mark();
    cdr()->mark();
  }
}

void Data::unmark() {
  marked = false;
}

bool Data::ismarked() const {
  return marked;
}
