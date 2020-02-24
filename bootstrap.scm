;;; Scheme bootstrapper for the Lispkit Lisp compiler
;;; as in P. Henderson's Functional Programming - Application and Implementation

;;; The compiler's bytecode is written to a file named lispkit.lkc.
;;; Note that for nicer interaction with Scheme, NILs are replaced by '()s.

;;; The bootstrapping is done in two phases:
;;; 1. Compile the original compiler with a compiler written in Scheme.
;;; 2. Compile the original compiler with its own compiled version, but
;;;    (i)  the input source code should use LispKit-style SEXPs (with NILs)
;;;    (ii) the output is converted to use ()s
;;; [The two compiled versions are exactly the same, of course, but it's a good test
;;;  to compile the compiler with itself.]

(load "secd.scm")

(define (lispkit-compile e)
  (comp e '() '(4 21)))                 ; compiled bytecode ends with AP STOP

(define (comp e n c)
  (if (not (pair? e)) (cons 1 (cons (location e n) c))                          ; LD (i.j)
      (case (car e)
        ((QUOTE) (cons 2 (cons (cadr e) c)))                                    ; LDC s
        ((ADD) (comp (cadr e) n (comp (caddr e) n (cons 15 c))))                ; e1 e2 ADD
        ((SUB) (comp (cadr e) n (comp (caddr e) n (cons 16 c))))                ; e1 e2 SUB
        ((MUL) (comp (cadr e) n (comp (caddr e) n (cons 17 c))))                ; e1 e2 MUL
        ((DIV) (comp (cadr e) n (comp (caddr e) n (cons 18 c))))                ; e1 e2 DIV
        ((REM) (comp (cadr e) n (comp (caddr e) n (cons 19 c))))                ; e1 e2 REM
        ((LEQ) (comp (cadr e) n (comp (caddr e) n (cons 20 c))))                ; e1 e2 LEQ
        ((EQ)  (comp (cadr e) n (comp (caddr e) n (cons 14 c))))                ; e1 e2 EQ
        ((CAR) (comp (cadr e) n (cons 10 c)))                                   ; e CAR
        ((CDR) (comp (cadr e) n (cons 11 c)))                                   ; e CDR
        ((ATOM) (comp (cadr e) n (cons 12 c)))                                  ; e ATOM
        ((CONS) (comp (caddr e) n (comp (cadr e) n (cons 13 c))))               ; e2 e1 CONS
        ((IF) (let ((thenpt (comp (caddr e) n '(9)))                            ; e1 (SEL (e2 JOIN)
                    (elsept (comp (cadddr e) n '(9))))                          ;         (e3 JOIN))
                (comp (cadr e) n (cons 8 (cons thenpt (cons elsept c))))))
        ((LAMBDA) (let ((body (comp (caddr e) (cons (cadr e) n) '(5))))         ; LDF (e RTN)
                    (cons 3 (cons body c))))                                    ; (w/extended env.)
        ((LET) (let ((m (cons (vars (cddr e)) n))                               ; = LAMBDA + appl.
                     (args (exprs (cddr e))))
                 (let ((body (comp (cadr e) m '(5))))
                   (complis args n (cons 3 (cons body (cons 4 c)))))))
        ((LETREC) (let ((m (cons (vars (cddr e)) n))                            ; = LET, but
                        (args (exprs (cddr e))))                                ; starts with DUM
                    (let ((body (comp (cadr e) m '(5))))                        ; and uses RAP
                      (cons 6 (complis args m (cons 3 (cons body (cons 7 c))))))))
        ; application: LDC NIL ek CONS ek-1 CONS ... e1 CONS e AP
        (else (complis (cdr e) n (comp (car e) n (cons 4 c)))))))

;;; Builds the argument list for function application
(define (complis e n c)
  (if (null? e)
      (cons 2 (cons 'NIL c))                                ; LDC NIL
      (complis (cdr e) n (comp (car e) n (cons 13 c)))))    ; ek CONS

;;; finds (i . j), where i is the index of the environment,
;;; and j is the index of the variable inside the i-th environment,
;;; where n is given as a list like '((e b f) (d a) (a b c))
(define (location e n)
  (let ((incar (lambda (l) (cons (+ 1 (car l)) (cdr l)))))
    (if (member e (car n))
        (cons 0 (posn e (car n)))
        (incar (location e (cdr n))))))

;;; finds the index of an element
(define (posn e n)
  (if (eq? e (car n))
      0
      (+ 1 (posn e (cdr n)))))

;;; takes the first elements from an alist
(define (vars d)
  (map car d))

;;; takes the second elements from an alist
(define (exprs d)
  (map cdr d))

;;; Exactly as in P. Henderson's Functional Programming - Application and Implementation
(define compiler
  '(LETREC COMPILE
     (COMPILE LAMBDA (E)
       (COMP E (QUOTE NIL) (QUOTE (4 21))))
     (COMP LAMBDA (E N C)
       (IF (ATOM E)
           (CONS (QUOTE 1) (CONS (LOCATION E N) C))
       (IF (EQ (CAR E) (QUOTE QUOTE))
           (CONS (QUOTE 2) (CONS (CAR (CDR E)) C))
       (IF (EQ (CAR E) (QUOTE ADD))
           (COMP (CAR (CDR E)) N (COMP (CAR (CDR (CDR E))) N (CONS (QUOTE 15) C)))
       (IF (EQ (CAR E) (QUOTE SUB))
           (COMP (CAR (CDR E)) N (COMP (CAR (CDR (CDR E))) N (CONS (QUOTE 16) C)))
       (IF (EQ (CAR E) (QUOTE MUL))
           (COMP (CAR (CDR E)) N (COMP (CAR (CDR (CDR E))) N (CONS (QUOTE 17) C)))
       (IF (EQ (CAR E) (QUOTE DIV))
           (COMP (CAR (CDR E)) N (COMP (CAR (CDR (CDR E))) N (CONS (QUOTE 18) C)))
       (IF (EQ (CAR E) (QUOTE REM))
           (COMP (CAR (CDR E)) N (COMP (CAR (CDR (CDR E))) N (CONS (QUOTE 19) C)))
       (IF (EQ (CAR E) (QUOTE LEQ))
           (COMP (CAR (CDR E)) N (COMP (CAR (CDR (CDR E))) N (CONS (QUOTE 20) C)))
       (IF (EQ (CAR E) (QUOTE EQ))
           (COMP (CAR (CDR E)) N (COMP (CAR (CDR (CDR E))) N (CONS (QUOTE 14) C)))
       (IF (EQ (CAR E) (QUOTE CAR))
           (COMP (CAR (CDR E)) N (CONS (QUOTE 10) C))
       (IF (EQ (CAR E) (QUOTE CDR))
           (COMP (CAR (CDR E)) N (CONS (QUOTE 11) C))
       (IF (EQ (CAR E) (QUOTE ATOM))
           (COMP (CAR (CDR E)) N (CONS (QUOTE 12) C))
       (IF (EQ (CAR E) (QUOTE CONS))
           (COMP (CAR (CDR (CDR E))) N (COMP (CAR (CDR E)) N (CONS (QUOTE 13) C)))
       (IF (EQ (CAR E) (QUOTE IF))
           (LET (COMP (CAR (CDR E)) N (CONS (QUOTE 8) (CONS THENPT (CONS ELSEPT C))))
             (THENPT COMP (CAR (CDR (CDR E))) N (QUOTE (9)))
             (ELSEPT COMP (CAR (CDR (CDR (CDR E)))) N (QUOTE  (9))))
       (IF (EQ (CAR E) (QUOTE LAMBDA))
           (LET (CONS (QUOTE 3) (CONS BODY C))
             (BODY COMP (CAR (CDR (CDR E))) (CONS (CAR (CDR E)) N) (QUOTE (5))))
       (IF (EQ (CAR E) (QUOTE LET))
           (LET (LET (COMPLIS ARGS N (CONS (QUOTE 3) (CONS BODY (CONS (QUOTE 4) C))))
                  (BODY COMP (CAR (CDR E)) M (QUOTE (5))))
             (M CONS (VARS (CDR (CDR E))) N)
             (ARGS EXPRS (CDR (CDR E))))
       (IF (EQ (CAR E) (QUOTE LETREC))
           (LET (LET (CONS (QUOTE 6)
                           (COMPLIS ARGS M (CONS (QUOTE 3) (CONS BODY (CONS (QUOTE 7) C)))))
                  (BODY COMP (CAR (CDR E)) M (QUOTE (5))))
             (M CONS (VARS (CDR (CDR E))) N)
             (ARGS EXPRS (CDR (CDR E))))
       (COMPLIS (CDR E) N (COMP (CAR E) N (CONS (QUOTE 4) C)))))))))))))))))))))
     (COMPLIS LAMBDA (E N C)
       (IF (EQ E (QUOTE NIL)) (CONS (QUOTE 2) (CONS (QUOTE NIL) C))
           (COMPLIS (CDR E) N (COMP (CAR E) N (CONS (QUOTE 13) C)))))
     (LOCATION LAMBDA (E N)
       (LETREC
         (IF (MEMBER E (CAR N)) (CONS (QUOTE 0) (POSN E (CAR N)))
             (INCAR (LOCATION E (CDR N))))
       (MEMBER LAMBDA (E N)
         (IF (EQ N (QUOTE NIL)) (QUOTE F)
             (IF (EQ E (CAR N)) (QUOTE T) (MEMBER E (CDR N)))))
       (POSN LAMBDA (E N)
         (IF (EQ E (CAR N)) (QUOTE 0) (ADD (QUOTE 1) (POSN E (CDR N)))))
       (INCAR LAMBDA (L) (CONS (ADD (QUOTE 1) (CAR L)) (CDR L)))))
     (VARS LAMBDA (D)
       (IF (EQ D (QUOTE NIL)) (QUOTE NIL)
           (CONS (CAR (CAR D))  (VARS (CDR D)))))
     (EXPRS LAMBDA (D)
       (IF (EQ D (QUOTE NIL)) (QUOTE NIL)
           (CONS (CDR (CAR D)) (EXPRS (CDR D)))))))

(with-output-to-file "bootstrap.lkc"
	(lambda ()
		(write (lispkit-compile compiler))))

(let ((bootstrap (lispkit-compile compiler)))
  (with-output-to-file "lispkit.lkc"
    (lambda ()
      (write (unnilify (exec bootstrap (nilify compiler)))))))
