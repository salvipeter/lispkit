#include "types.hh"

enum class DataType { SYMBOL, NUMBER, CONS };

struct Data {
  Data(DataType type) : type(type) { }
  virtual ~Data() { }
  DataType type;
};

struct Symbol : public Data {
  Symbol(std::string svalue) : Data(DataType::SYMBOL), svalue(svalue) { }
  ~Symbol() { }
  std::string svalue;
};

struct Number : public Data {
  Number(int ivalue) : Data(DataType::NUMBER), ivalue(ivalue) { }
  ~Number()  { }
  int ivalue;
};

struct Cons : public Data {
  Cons(Data *car, Data *cdr) : Data(DataType::CONS), car(car), cdr(cdr) { }
  ~Cons() { dispose(car); dispose(cdr); }
  Data *car, *cdr;
};

void dispose(Data *d) {
  if (d != nil && d != bool_t && d != bool_f)
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

Symbol nil_symbol("NIL"), bool_t_symbol("T"), bool_f_symbol("F");
Data *nil = &nil_symbol, *bool_t = &bool_t_symbol, *bool_f = &bool_f_symbol;
