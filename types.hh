#pragma once

#include <string>

// Section 11.1

enum class DataType { SYMBOL, NUMBER, CONS };

struct Data {
  Data(DataType type) : type(type) { }
  virtual ~Data() { }

  bool issymbol();
  bool isnumber();
  bool iscons();

private:
  DataType type;
};

extern void dispose(Data *d);

struct Symbol : public Data {
  Symbol(std::string svalue) : Data(DataType::SYMBOL), svalue(svalue) { }
  ~Symbol() { }
  std::string svalue;
};

extern Symbol nil;

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
