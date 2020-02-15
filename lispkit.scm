;;; (compile-and-run program . args) takes the source of a LispKit program
;;; compiles it to bytecode, and then calls exec (see secd.scm).

;;; Since this compiler uses Scheme-style SEXPs ending with '() instead of NIL,
;;; all NILs in the program code are automatically converted to '() first.

;;; Example:
;;; (compile-and-run fact 10) ; fact is defined in tests.scm

(load "secd.scm")

(define compiler-bytecode
  (with-input-from-file "lispkit.lkc"
    (lambda ()
      (read))))

(define (compile-and-run program . args)
  (let ((bytecode (exec compiler-bytecode (unnilify program))))
    (apply exec bytecode args)))
