# LispKit

A Scheme implementation of the SECD virtual machine, and a LispKit Lisp compiler, based on
P. Henderson's Functional Programming - Application and Implementation.

First generate the compiler bytecode (you only need to do this once):

```scheme
> (load "bootstrap.scm")
```

Then you can load the compiler/VM:

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
