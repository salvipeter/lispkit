#include "types.hh"

#include <algorithm>
#include <stdexcept>
#include <vector>

enum class DataType { SYMBOL, NUMBER, CONS };

struct Data {
  Data(size_t s) : type(DataType::SYMBOL), svalue(s) { }
  Data(Data *a, Data *b) : type(DataType::CONS), cadr({ a, b }) { }
  ~Data() { }

  DataType type;
  union {
    size_t svalue;
    int ivalue;
    std::pair<Data *, Data *> cadr;
  };
};

// Memory handling

namespace {
  struct Pool {
    std::vector<Data*> data;
    std::vector<std::string> strings;
    Data *free_list;
  } pool;
}

void initialize(size_t size) {
  pool.data.resize(size);
  pool.strings = { "NIL", "T", "F" };
  pool.free_list = nil;
  for (auto &d : pool.data) {
    d = new Data(nil, pool.free_list);
    pool.free_list = d;
  }
}

void shutdown() {
  for (auto d : pool.data)
    delete d;
  pool.data.clear();
  pool.strings.clear();
}

static void gc() {
}

static Data *getNextCell() {
  if (pool.free_list == nil) {
    gc();
    if (pool.free_list == nil)
      throw std::runtime_error("out of memory");
  }
  auto d = pool.free_list;
  pool.free_list = cdr(d);
  return d;
}

// Interface implementation

Data *symbol(std::string s) {
  auto d = getNextCell();
  d->type = DataType::SYMBOL;
  auto iter = std::find(pool.strings.begin(), pool.strings.end(), s);
  size_t index = iter - pool.strings.begin();
  if (iter == pool.strings.end())
    pool.strings.push_back(s);
  d->svalue = index;
  return d;
}

Data *number(int n) {
  auto d = getNextCell();
  d->type = DataType::NUMBER;
  d->ivalue = n;
  return d;
}

Data *cons(Data *a, Data *b) {
  auto d = getNextCell();
  d->type = DataType::CONS;
  d->cadr = { a, b };
  return d;
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
  return pool.strings[d->svalue];
}

int ivalue(Data *d) {
  return d->ivalue;
}

Data *car(Data *d) {
  return d->cadr.first;
}

Data *cdr(Data *d) {
  return d->cadr.second;
}

void rplaca(Data *c, Data *d) {
  c->cadr.first = d;
}

Data nil_symbol(0), bool_t_symbol(1), bool_f_symbol(2);

Data *nil = &nil_symbol, *bool_t = &bool_t_symbol, *bool_f = &bool_f_symbol;
