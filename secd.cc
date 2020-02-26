// A quite literal rendition of the machine described in Chapters 11-12
// of Henderson's Functional Programming - Application and Implementation

#include <fstream>

#include "reader.hh"

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <function.lkc> <arguments.lkl>" << std::endl;
    return 1;
  }

  std::ifstream f_fn(argv[1]), f_args(argv[2]);
  f_fn.exceptions(std::ios::failbit | std::ios::badbit);
  f_args.exceptions(std::ios::failbit | std::ios::badbit);
  // Data *fn = getexp(f_fn), *args = getexplist(f_args);
  // Data *result = exec(fn, args);
  // putexp(result, std::cout);
  // dispose(result);
  // dispose(args);
  Data *fn = getexp(f_fn);
  putexp(fn, std::cout);
  std::cout << std::endl;
  dispose(fn);
}
