#pragma once

#include <string>

// Section 11.1

enum class DataType { SYMBOL, NUMBER, CONS };

struct Data {
  Data(DataType type) : type(type) { }
  virtual ~Data() { }
  DataType type;
};

extern void dispose(Data *d);

struct Symbol : public Data {
  Symbol(std::string svalue) : Data(DataType::SYMBOL), svalue(svalue) { }
  ~Symbol() { }
  std::string svalue;
};

extern Symbol nil;
extern Symbol bool_t, bool_f; // Section 11.8

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

// Simple interface

Data *symbol(std::string s);
Data *number(int n);
Data *cons(Data *a, Data *b);

bool issymbol(Data *d);
bool isnumber(Data *d);
bool iscons(Data *d);

std::string svalue(Data *d);
int ivalue(Data *d);
Data *car(Data *d);
Data *cdr(Data *d);
void rplaca(Data *c, Data *d);
