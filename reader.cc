#include <exception>

#include "reader.hh"

Data *getexp(const Token &token, std::istream &is) {
  switch (token.type) {
  case TokenType::NUMERIC:
    return number(std::atoi(token.value.c_str()));
  case TokenType::ALPHANUMERIC:
    return symbol(token.value);
  case TokenType::DELIMITER:    // should be "("
    return getexplist(is);
  case TokenType::ENDFILE:
    throw std::runtime_error("premature end of file");
  }
  return nullptr;               // cannot reach this
}

Data *getexp(std::istream &is) {
  auto token = gettoken(is);
  return getexp(token, is);
}

Data *getexplist(std::istream &is, Data *next_car) {
  Data *car, *cdr;
  if (next_car)
    car = next_car;
  else
    car = getexp(is);
  auto token = gettoken(is);
  if (token.value == ".") {
    cdr = getexp(is);
    gettoken(is);               // read also the final ')'
  }
  else if (token.type == TokenType::ENDFILE || token.value == ")")
    cdr = nil;
  else {
    auto next = getexp(token, is);
    cdr = getexplist(is, next);
  }
  return cons(car, cdr);
}

Token gettoken(std::istream &is) {
  is >> std::ws;
  if (is.eof())
    return { TokenType::ENDFILE, "" };
  char c;
  is.get(c);
  if (std::isdigit(c) || c == '-') {
    std::string s(1, c);
    while (!is.eof()) {
      c = is.peek();
      if (c == std::char_traits<char>::eof() || !std::isdigit(c))
        break;
      is.get(c);
      s += c;
    }
    return { TokenType::NUMERIC, s };
  }
  if (std::isalpha(c)) {
    std::string s(1, c);
    while (!is.eof()) {
      c = is.peek();
      if (c == std::char_traits<char>::eof() || !(std::isalpha(c) || std::isdigit(c)))
        break;
      is.get(c);
      s += c;
    }
    return { TokenType::ALPHANUMERIC, s };
  }
  return { TokenType::DELIMITER, { c } };
}

// There are only 3 cases when we do not want space before printing something:
// - it is the first thing we print
// - it is immediately after a '('
// - it is a ')'
void putexp(Data *e, std::ostream &os) {
  if (issymbol(e))
    os << svalue(e);
  else if (isnumber(e))
    os << ivalue(e);
  else {
    os << '(';
    auto p = e;
    while (iscons(p)) {
      putexp(car(p), os);
      p = cdr(p);
      if (iscons(p))
        os << ' ';
    }
    if (p != nil) {
      os << " . ";
      putexp(p, os);
    }
    os << ')';
  }
}
