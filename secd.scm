;;; SECD virtual machine for LispKit Lisp
;;; as in P. Henderson's Functional Programming - Application and Implementation

;;; (exec f . args) takes a function in bytecode and applies it to the arguments.

(load "utils.scm")

(define (exec f . args)
  (let rec ((state (list (list args) '() f '())))
    (if (stop? state)
        (caar state)                    ; Return the top of the stack
        (rec (apply secd state)))))

(define (stop? state)
  (eq? (car (caddr state)) 21))

(define (secd s e c d)
  ((vector-ref operators (car c)) s e (cdr c) d))

(define operators (make-vector 22))     ; 0-th element not used

(define-syntax defop
  (syntax-rules ()
    ((_ code name (s e c d) body ...)
     (let ((name (lambda (s e c d) body ...)))
       (vector-set! operators code name)))))

(defop 1 LD (s e c d)
  (let ((locate (lambda (i e) (list-ref (list-ref e (car i)) (cdr i)))))
    (list (cons (locate (car c) e) s) e (cdr c) d)))

(defop 2 LDC (s e c d)
  (list (cons (car c) s) e (cdr c) d))

(defop 3 LDF (s e c d)
  (list (cons (cons (car c) e) s) e (cdr c) d))

(defop 4 AP (s e c d)
  (list '() (cons (cadr s) (cdar s)) (caar s) (cons (cddr s) (cons e (cons c d)))))

(defop 5 RTN (s e c d)
  (list (cons (car s) (car d)) (cadr d) (caddr d) (cdddr d)))

(defop 6 DUM (s e c d)
  (list s (cons 'pending e) c d))

(defop 7 RAP (s e c d)
  (set-car! (cdar s) (cadr s))
  (list '() (cdar s) (caar s) (cons (cddr s) (cons (cdr e) (cons c d)))))

(defop 8 SEL (s e c d)
  (list (cdr s) e (if (eq? (car s) 'F) (cadr c) (car c)) (cons (cddr c) d)))

(defop 9 JOIN (s e c d)
  (list s e (car d) (cdr d)))

(defop 10 CAR (s e c d)
  (list (cons (car (car s)) (cdr s)) e c d))

(defop 11 CDR (s e c d)
  (list (cons (cdr (car s)) (cdr s)) e c d))

(defop 12 ATOM (s e c d)
  (list (cons (if (pair? (car s)) 'F 'T) (cdr s)) e c d))

(defop 13 CONS (s e c d)
  (list (cons (cons (car s) (cadr s)) (cddr s)) e c d))

(defop 14 EQ (s e c d)
  (list (cons (if (eq? (cadr s) (car s)) 'T 'F) (cddr s)) e c d))

(defop 15 ADD (s e c d)
  (list (cons (+ (cadr s) (car s)) (cddr s)) e c d))

(defop 16 SUB (s e c d)
  (list (cons (- (cadr s) (car s)) (cddr s)) e c d))

(defop 17 MUL (s e c d)
  (list (cons (* (cadr s) (car s)) (cddr s)) e c d))

(defop 18 DIV (s e c d)
  (list (cons (quotient (cadr s) (car s)) (cddr s)) e c d))

(defop 19 REM (s e c d)
  (list (cons (remainder (cadr s) (car s)) (cddr s)) e c d))

(defop 20 LEQ (s e c d)
  (list (cons (if (<= (cadr s) (car s)) 'T 'F) (cddr s)) e c d))
