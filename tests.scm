(define fact
  '(LETREC FACT
     (FACT LAMBDA (N)
       (IF (LEQ N (QUOTE 0))
           (QUOTE 1)
           (MUL N (FACT (SUB N (QUOTE 1))))))))

(define evenp
  '(LETREC EVENP
     (EVENP LAMBDA (N)
       (IF (EQ N (QUOTE 0))
           (QUOTE T)
           (IF (EQ N (QUOTE 1))
               (QUOTE F)
               (ODDP (SUB N (QUOTE 1))))))
     (ODDP LAMBDA (N)
       (IF (EQ N (QUOTE 1))
           (QUOTE T)
           (IF (EQ N (QUOTE 0))
               (QUOTE F)
               (EVENP (SUB N (QUOTE 1))))))))

(define rev
  '(LETREC REVERSE
     (REVERSE LAMBDA (X)
       (IF (EQ X (QUOTE NIL))
           (QUOTE NIL)
           (APPEND (REVERSE (CDR X))
                   (CONS (CAR X) (QUOTE NIL)))))
     (APPEND LAMBDA (X Y)
       (IF (EQ X (QUOTE NIL))
           Y
           (CONS (CAR X) (APPEND (CDR X) Y))))))
