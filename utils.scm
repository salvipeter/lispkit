(define (nilify expr)                   ; convert to lists ending with NIL
  (let rec ((expr expr))
    (cond ((pair? expr) (cons (nilify (car expr)) (nilify (cdr expr))))
          ((null? expr) 'NIL)
          (else expr))))

(define (unnilify expr)                 ; convert from lists ending with NIL
  (let rec ((expr expr))
    (cond ((pair? expr) (cons (unnilify (car expr)) (unnilify (cdr expr))))
          ((eq? expr 'NIL) '())
          (else expr))))
