#include <fstream>

#include "secd.hh"

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <function.lkc> <arguments.lkl> [memory_limit]"
              << std::endl;
    return 1;
  }

  size_t memory_limit = 10000;
  if (argc == 4)
    memory_limit = std::atoi(argv[3]);

  SECD secd(memory_limit);

  // Section 11.2
  std::ifstream f_fn(argv[1]), f_args(argv[2]);
  f_fn.exceptions(std::ios::failbit | std::ios::badbit);
  f_args.exceptions(std::ios::failbit | std::ios::badbit);
  Data *fn = secd.getexp(f_fn), *args = secd.getexplist(f_args);
  Data *result = secd.exec(fn, args);
  secd.putexp(result, std::cout);
  std::cout << std::endl;
}
