(define (domain blocks)
(:predicates
    (a)
    (b)
    (c)
    (d)
    (e)
    (f)
    (g)
    (h)
)
(:action first_0
	:parameters ()
	:precondition (and
		(a)
	)
	:effect (and
		(not(a))(b)
	)
)

(:action first_1
	:parameters ()
	:precondition (and
		(a)
	)
	:effect (and
		(not(a))(c)
	)
)

(:action second_0
	:parameters ()
	:precondition (and
		(b)
	)
	:effect (and
		(not(b))(d)
	)
)
(:action second_1
	:parameters ()
	:precondition (and
		(b)
	)
	:effect (and
		(not(b))(e)
	)
)
(:action second_2
	:parameters ()
	:precondition (and
		(c)
	)
	:effect (and
		(not(c))(f)
	)
)
(:action second_3
	:parameters ()
	:precondition (and
		(c)
	)
	:effect (and
		(not(c))(h)
	)
)
(:action third_0
	:parameters ()
	:precondition (and
		(d)
	)
	:effect (and
		(not(d))(g)
	)
)
(:action third_1
	:parameters ()
	:precondition (and
		(e)
	)
	:effect (and
		(not(e))(g)
	)
)
(:action third_2
	:parameters ()
	:precondition (and
		(f)
	)
	:effect (and
		(not(f))(g)
	)
)
(:action third_3
	:parameters ()
	:precondition (and
		(h)
	)
	:effect (and
		(not(h))(g)
	)
)
)