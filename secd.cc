#include "secd.hh"
#include "types.hh"

#include <algorithm>
#include <stdexcept>

#define PRINT_MEMORY_INFO

SECD::SECD(size_t memory_limit) : running(false) {
  strings = { "NIL", "T", "F" };
  nil = new Data(0); t = new Data(1); f = new Data(2);
  data.resize(memory_limit);
  free_list = nil;
  for (auto &p : data) {
    p = new Data(nil, free_list);
    free_list = p;
  }
}

SECD::~SECD() {
#ifdef PRINT_MEMORY_INFO
  std::cout << data.size() - memory() << " cells still in use at exit." << std::endl;
#endif
  for (auto p : data)
    delete p;
  delete f;
  delete t;
  delete nil;
}

size_t SECD::memory() const {
  size_t counter = 0;
  auto p = free_list;
  while (p != nil) {
    ++counter;
    p = p->cdr();
  }
  return counter;
}

Data *SECD::getexp(std::istream &is) {
  auto token = gettoken(is);
  return getexp(token, is);
}

Data *SECD::getexp(const Token &token, std::istream &is) {
  switch (token.type) {
  case Token::TokenType::NUMERIC:
    return number(std::atoi(token.value.c_str()));
  case Token::TokenType::ALPHANUMERIC:
    return symbol(token.value);
  case Token::TokenType::DELIMITER:    // should be "("
    return getexplist(is);
  case Token::TokenType::ENDFILE:
    throw std::runtime_error("premature end of file");
  }
  return nullptr;               // cannot reach this
}

SECD::Token SECD::gettoken(std::istream &is) {
  is >> std::ws;
  if (is.eof())
    return { Token::TokenType::ENDFILE, "" };
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
    return { Token::TokenType::NUMERIC, s };
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
    return { Token::TokenType::ALPHANUMERIC, s };
  }
  return { Token::TokenType::DELIMITER, { c } };
}

Data *SECD::getexplist(std::istream &is) {
  return getexplist(is, nullptr);
}

Data *SECD::getexplist(std::istream &is, Data *next_car) {
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
  else if (token.type == Token::TokenType::ENDFILE || token.value == ")")
    cdr = nil;
  else {
    auto next = getexp(token, is);
    cdr = getexplist(is, next);
  }
  return cons(car, cdr);
}

// There are only 3 cases when we do not want space before printing something:
// - it is the first thing we print
// - it is immediately after a '('
// - it is a ')'
void SECD::putexp(Data *e, std::ostream &os) const {
  if (e->issymbol())
    os << e->svalue();
  else if (e->isnumber())
    os << e->ivalue();
  else {
    os << '(';
    auto p = e;
    while (p->iscons()) {
      putexp(p->car(), os);
      p = p->cdr();
      if (p->iscons())
        os << ' ';
    }
    if (p != nil) {
      os << " . ";
      putexp(p, os);
    }
    os << ')';
  }
}

Data *SECD::exec(Data *fn, Data *args) {
  s = cons(args, nil); e = nil; c = fn; d = nil;
  running = true;
  while (running) {
    switch (c->car()->ivalue()) {
    case 1:  // LD
      w = e;
      for (int i = 1; i <= c->cdr()->car()->car()->ivalue(); ++i)
        w = w->cdr();
      w = w->car();
      for (int i = 1; i <= c->cdr()->car()->cdr()->ivalue(); ++i)
        w = w->cdr();
      w = w->car();
      s = cons(w, s);
      c = c->cdr()->cdr();
      break;
    case 2:  // LDC
      s = cons(c->cdr()->car(), s);
      c = c->cdr()->cdr();
      break;
    case 3:  // LDF
      s = cons(cons(c->cdr()->car(), e), s);
      c = c->cdr()->cdr();
      break;
    case 4:  // AP
      d = cons(s->cdr()->cdr(), cons(e, cons(c->cdr(), d)));
      e = cons(s->cdr()->car(), s->car()->cdr());
      c = s->car()->car();
      s = nil;
      break;
    case 5:  // RTN
      s = cons(s->car(), d->car());
      e = d->cdr()->car();
      c = d->cdr()->cdr()->car();
      d = d->cdr()->cdr()->cdr();
      break;
    case 6:  // DUM
      e = cons(nil, e);
      c = c->cdr();
      break;
    case 7:  // RAP
      d = cons(s->cdr()->cdr(), cons(e->cdr(), cons(c->cdr(), d)));
      e = s->car()->cdr();
      e->rplaca(s->cdr()->car());
      c = s->car()->car();
      s = nil;
      break;
    case 8:  // SEL
      d = cons(c->cdr()->cdr()->cdr(), d);
      if (s->car()->svalue() == 1) // "T"
        c = c->cdr()->car();
      else
        c = c->cdr()->cdr()->car();
      s = s->cdr();
      break;
    case 9:  // JOIN
      c = d->car();
      d = d->cdr();
      break;
    case 10: // CAR
      s = cons(s->car()->car(), s->cdr());
      c = c->cdr();
      break;
    case 11: // CDR
      s = cons(s->car()->cdr(), s->cdr());
      c = c->cdr();
      break;
    case 12: // ATOM
      if (s->car()->isnumber() || s->car()->issymbol())
        s = cons(t, s->cdr());
      else
        s = cons(f, s->cdr());
      c = c->cdr();
      break;
    case 13: // CONS
      s = cons(cons(s->car(), s->cdr()->car()), s->cdr()->cdr());
      c = c->cdr();
      break;
    case 14: // EQ
      if ((s->car()->issymbol() && s->cdr()->car()->issymbol() &&
           s->car()->svalue() == s->cdr()->car()->svalue()) ||
          (s->car()->isnumber() && s->cdr()->car()->isnumber() &&
           s->car()->ivalue() == s->cdr()->car()->ivalue()))
        s = cons(t, s->cdr()->cdr());
      else
        s = cons(f, s->cdr()->cdr());
      c = c->cdr();
      break;
    case 15: // ADD
      s = cons(number(s->cdr()->car()->ivalue() + s->car()->ivalue()), s->cdr()->cdr());
      c = c->cdr();
      break;
    case 16: // SUB
      s = cons(number(s->cdr()->car()->ivalue() - s->car()->ivalue()), s->cdr()->cdr());
      c = c->cdr();
      break;
    case 17: // MUL
      s = cons(number(s->cdr()->car()->ivalue() * s->car()->ivalue()), s->cdr()->cdr());
      c = c->cdr();
      break;
    case 18: // DIV
      s = cons(number(s->cdr()->car()->ivalue() / s->car()->ivalue()), s->cdr()->cdr());
      c = c->cdr();
      break;
    case 19: // REM
      s = cons(number(s->cdr()->car()->ivalue() % s->car()->ivalue()), s->cdr()->cdr());
      c = c->cdr();
      break;
    case 20: // LEQ
      if (s->cdr()->car()->ivalue() <= s->car()->ivalue())
        s = cons(t, s->cdr()->cdr());
      else
        s = cons(f, s->cdr()->cdr());
      c = c->cdr();
      break;
    case 21: // STOP
      running = false;
      break;
    }
  }
  return s->car();
}

Data *SECD::getNextCell() {
  if (free_list == nil) {
    if (running) {              // Cannot garbage collect in the reading phase
      gc();
#ifdef PRINT_MEMORY_INFO
      std::cout << "Garbage collected " << memory() << " cells." << std::endl;
#endif
    }
    if (free_list == nil)
      throw std::runtime_error("out of memory");
  }
  auto p = free_list;
  free_list = p->cdr();
  return p;
}

void SECD::gc() {
  for (auto p : data)
    p->unmark();

  s->mark(); e->mark(); c->mark(); d->mark();
  w->mark(); t->mark(); f->mark(); nil->mark();

  for (auto p : data)
    if (!p->ismarked()) {
      p->setCons(nil, free_list);
      free_list = p;
    }
}

Data *SECD::symbol(std::string s) {
  auto p = getNextCell();
  auto iter = std::find(strings.begin(), strings.end(), s);
  size_t index = iter - strings.begin();
  if (iter == strings.end())
    strings.push_back(s);
  p->setSymbol(index);
  return p;
}

Data *SECD::number(int n) {
  auto p = getNextCell();
  p->setNumber(n);
  return p;
}

Data *SECD::cons(Data *a, Data *b) {
  auto p = getNextCell();
  p->setCons(a, b);
  return p;
}
