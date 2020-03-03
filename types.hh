#pragma once

#include <cstddef>
#include <utility>

// Section 11.1

class Data {
public:
  Data(size_t s);
  Data(Data *a, Data *b);
  ~Data();

  void setSymbol(size_t s);
  size_t svalue() const;

  void setNumber(int n);
  int ivalue() const;

  void setCons(Data *a, Data *b);
  Data *car() const;
  Data *cdr() const;
  void rplaca(Data *d);

  bool issymbol() const;
  bool isnumber() const;
  bool iscons() const;

  // Garbage collection mark handling                    Section 12.2
  void mark();
  void unmark();
  bool ismarked() const;

private:
  enum class DataType { SYMBOL, NUMBER, CONS } type;
  bool marked;
  union {
    size_t str_index;
    int number;
    std::pair<Data *, Data *> cadr;
  };
};
