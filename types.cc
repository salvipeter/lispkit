#include "types.hh"

void dispose(Data *d) {
  if (d != &nil && d != &bool_t && d != &bool_f)
    delete d;
}

Data *symbol(std::string s) {
  return new Symbol(s);
}

Data *number(int n) {
  return new Number(n);
}

Data *cons(Data *a, Data *b) {
  return new Cons(a, b);
}

bool issymbol(Data *d) {
  return d->type == DataType::SYMBOL;
}

bool isnumber(Data *d) {
  return d->type == DataType::NUMBER;
}

bool iscons(Data *d) {
  return d->type == DataType::CONS;
}

std::string svalue(Data *d) {
  return dynamic_cast<Symbol*>(d)->svalue;
}

int ivalue(Data *d) {
  return dynamic_cast<Number*>(d)->ivalue;
}

Data *car(Data *d) {
  return dynamic_cast<Cons*>(d)->car;
}

Data *cdr(Data *d) {
  return dynamic_cast<Cons*>(d)->cdr;
}

void rplaca(Data *c, Data *d) {
  dynamic_cast<Cons*>(c)->car = d;
}

Symbol nil("NIL");
Symbol bool_t("T"), bool_f("F");
