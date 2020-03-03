# LispKit

A Scheme and a C++ implementation of the SECD virtual machine, and a LispKit Lisp compiler,
based on P. Henderson's Functional Programming - Application and Implementation.

First generate the compiler bytecode (you only need to do this once):

```scheme
> (load "bootstrap.scm")
```

This will generate two files: `lispkit.lkc`, which will be used in the Scheme version,
and `bootstrap.lkc`, which will be used in the C++ version.

## Scheme version

You can load the compiler/VM with:

```scheme
> (load "lispkit.scm")
```

There are a few tests, as well:

```scheme
> (load "tests.scm")
> (compile-and-run fact 10)
3628800
> (compile-and-run rev '(1 2 3 4))
(4 3 2 1)
```

## C++ version

Use `make` to build the program. There are 3 targets: `debug`, `release`, and `clean`.
The debug build also prints information on the garbage collector.

Here is an example session:

```bash
$ make debug
g++ -Wall -std=c++17 -pedantic -g -DDEBUG -DPRINT_MEMORY_INFO   -c -o lispkit.o lispkit.cc
g++ -Wall -std=c++17 -pedantic -g -DDEBUG -DPRINT_MEMORY_INFO   -c -o secd.o secd.cc
g++ -Wall -std=c++17 -pedantic -g -DDEBUG -DPRINT_MEMORY_INFO   -c -o types.o types.cc
g++ -o lispkit lispkit.o secd.o types.o
$ ./lispkit
Usage: ./lispkit <function.lkc> <arguments.lkl> [memory_limit]
$ cat fact.lkl
(LETREC FACT
     (FACT LAMBDA (N)
       (IF (LEQ N (QUOTE 0))
           (QUOTE 1)
           (MUL N (FACT (SUB N (QUOTE 1)))))))
$ ./lispkit bootstrap.lkc fact.lkl > fact.lkc
4172 cells still in use at exit.
$ cat input.lkl
10
$ ./lispkit fact.lkc input.lkl
3628800
314 cells still in use at exit.
$ ./lispkit fact.lkc input.lkl 160
Garbage collected 63 cells.
Garbage collected 34 cells.
Garbage collected 18 cells.
Garbage collected 9 cells.
Garbage collected 12 cells.
Garbage collected 44 cells.
3628800
134 cells still in use at exit.
$
```
