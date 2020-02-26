// A quite literal rendition of the machine described in Chapters 11-12
// of Henderson's Functional Programming - Application and Implementation

#include <fstream>

#include "reader.hh"

// Section 11.8

Data *exec(Data *fn, Data *args) {
  Data *s = cons(args, nil), *e = nil, *c = fn, *d = nil, *w;
  bool stop = false;
  while (!stop) {
    switch (ivalue(car(c))) {
    case 1:  // LD
      w = e;
      for (int i = 1; i <= ivalue(car(car(cdr(c)))); ++i)
        w = cdr(w);
      w = car(w);
      for (int i = 1; i <= ivalue(cdr(car(cdr(c)))); ++i)
        w = cdr(w);
      w = car(w);
      s = cons(w, s);
      c = cdr(cdr(c));
      break;
    case 2:  // LDC
      s = cons(car(cdr(c)), s);
      c = cdr(cdr(c));
      break;
    case 3:  // LDF
      s = cons(cons(car(cdr(c)), e), s);
      c = cdr(cdr(c));
      break;
    case 4:  // AP
      d = cons(cdr(cdr(s)), cons(e, cons(cdr(c), d)));
      e = cons(car(cdr(s)), cdr(car(s)));
      c = car(car(s));
      s = nil;
      break;
    case 5:  // RTN
      s = cons(car(s), car(d));
      e = car(cdr(d));
      c = car(cdr(cdr(d)));
      d = cdr(cdr(cdr(d)));
      break;
    case 6:  // DUM
      e = cons(nil, e);
      c = cdr(c);
      break;
    case 7:  // RAP
      d = cons(cdr(cdr(s)), cons(cdr(e), cons(cdr(c), d)));
      e = cdr(car(s));
      rplaca(e, car(cdr(s)));
      c = car(car(s));
      s = nil;
      break;
    case 8:  // SEL
      d = cons(cdr(cdr(cdr(c))), d);
      if (svalue(car(s)) == "T")
        c = car(cdr(c));
      else
        c = car(cdr(cdr(c)));
      s = cdr(s);
      break;
    case 9:  // JOIN
      c = car(d);
      d = cdr(d);
      break;
    case 10: // CAR
      s = cons(car(car(s)), cdr(s));
      c = cdr(c);
      break;
    case 11: // CDR
      s = cons(cdr(car(s)), cdr(s));
      c = cdr(c);
      break;
    case 12: // ATOM
      if (isnumber(car(s)) || issymbol(car(s)))
        s = cons(bool_t, cdr(s));
      else
        s = cons(bool_f, cdr(s));
      c = cdr(c);
      break;
    case 13: // CONS
      s = cons(cons(car(s), car(cdr(s))), cdr(cdr(s)));
      c = cdr(c);
      break;
    case 14: // EQ
      if ((issymbol(car(s)) && issymbol(car(cdr(s))) && svalue(car(s)) == svalue(car(cdr(s)))) ||
          (isnumber(car(s)) && isnumber(car(cdr(s))) && ivalue(car(s)) == ivalue(car(cdr(s)))))
        s = cons(bool_t, cdr(cdr(s)));
      else
        s = cons(bool_f, cdr(cdr(s)));
      c = cdr(c);
      break;
    case 15: // ADD
      s = cons(number(ivalue(car(cdr(s))) + ivalue(car(s))), cdr(cdr(s)));
      c = cdr(c);
      break;
    case 16: // SUB
      s = cons(number(ivalue(car(cdr(s))) - ivalue(car(s))), cdr(cdr(s)));
      c = cdr(c);
      break;
    case 17: // MUL
      s = cons(number(ivalue(car(cdr(s))) * ivalue(car(s))), cdr(cdr(s)));
      c = cdr(c);
      break;
    case 18: // DIV
      s = cons(number(ivalue(car(cdr(s))) / ivalue(car(s))), cdr(cdr(s)));
      c = cdr(c);
      break;
    case 19: // REM
      s = cons(number(ivalue(car(cdr(s))) % ivalue(car(s))), cdr(cdr(s)));
      c = cdr(c);
      break;
    case 20: // LEQ
      if (ivalue(car(cdr(s))) <= ivalue(car(s)))
        s = cons(bool_t, cdr(cdr(s)));
      else
        s = cons(bool_f, cdr(cdr(s)));
      c = cdr(c);
      break;
    case 21: // STOP
      stop = true;
      break;
    }
  }
  return car(s);
}

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <function.lkc> <arguments.lkl>" << std::endl;
    return 1;
  }

  // Section 11.2
  std::ifstream f_fn(argv[1]), f_args(argv[2]);
  f_fn.exceptions(std::ios::failbit | std::ios::badbit);
  f_args.exceptions(std::ios::failbit | std::ios::badbit);
  Data *fn = getexp(f_fn), *args = getexplist(f_args);
  Data *result = exec(fn, args);
  putexp(result, std::cout);
  std::cout << std::endl;
  // dispose(result);
  // dispose(args);
  // dispose(fn);
}
