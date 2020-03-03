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

  // Reader/Printer                                      Sections 11.3,5
  Data *getexp(std::istream &is);
  Data *getexplist(std::istream &is);
  void putexp(Data *e, std::ostream &os) const;

  // VM execution                                        Section 11.8
  Data *exec(Data *fn, Data *args);

private:
  // Memory handling                                     Section 12.2
  void gc();
  Data *getNextCell();

  // Cell constructors                                   Section 11.1
  Data *symbol(std::string s);
  Data *number(int n);
  Data *cons(Data *a, Data *b);

  // Reader utilities                                    Sections 11.3-4
  struct Token {
    enum class TokenType { ALPHANUMERIC, NUMERIC, DELIMITER, ENDFILE } type;
    std::string value;
  };
  Data *getexp(const Token &token, std::istream &is);
  Data *getexplist(std::istream &is, Data *next_car);
  static Token gettoken(std::istream &is);

  // Machine state                                       Section 11.8
  Data *s, *e, *c, *d;
  Data *w, *t, *f, *nil;

  bool running;

  // Memory pool                                         Section 12.1
  std::vector<Data *> data;
  Data *free_list;

  // String store                                        Section 12.3
  std::vector<std::string> strings;
};
