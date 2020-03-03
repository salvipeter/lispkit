#pragma once

#include <iostream>
#include <string>
#include <vector>

class Data;

class SECD {
public:
  SECD(size_t memory_limit);
  ~SECD();
  size_t memory() const;

  Data *getexp(std::istream &is);
  Data *getexplist(std::istream &is);
  void putexp(Data *e, std::ostream &os) const; // Section 11.5

  Data *exec(Data *fn, Data *args); // Section 11.8

private:
  // Memory handling
  void gc();
  Data *getNextCell();

  // Cell constructors
  Data *symbol(std::string s);
  Data *number(int n);
  Data *cons(Data *a, Data *b);

  // Reader utilities
  struct Token {
    enum class TokenType { ALPHANUMERIC, NUMERIC, DELIMITER, ENDFILE } type;
    std::string value;
  };
  Data *getexp(const Token &token, std::istream &is);
  Data *getexplist(std::istream &is, Data *next_car);
  static Token gettoken(std::istream &is); // Section 11.4

  // Machine state
  Data *s, *e, *c, *d;
  Data *w, *t, *f, *nil;

  std::vector<Data *> data;
  std::vector<std::string> strings;
  Data *free_list;
};
