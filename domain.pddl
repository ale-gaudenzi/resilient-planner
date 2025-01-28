(define (domain blocks)
(:predicates
	(a)
	(b)
	(c)
)

(:action move_from_a_1
	:parameters ()
	:precondition (and
		(a)
	)
	:effect (and
		(not (a))
		(b)
	)
)
(:action move_from_a_2
	:parameters ()
	:precondition (and
		(a)
	)
	:effect (and
		(not (a))
		(b)
	)
)(:action move_from_a_3
	:parameters ()
	:precondition (and
		(a)
	)
	:effect (and
		(not (a))
		(b)
	)
)

(:action move_from_b_1
	:parameters ()
	:precondition (and
		(b)
	)
	:effect (and
		(not (b))
		(c)
	)
)
(:action move_from_b_2
	:parameters ()
	:precondition (and
		(b)
	)
	:effect (and
		(not (b))
		(c)
	)
)(:action move_from_b_3
	:parameters ()
	:precondition (and
		(b)
	)
	:effect (and
		(not (b))
		(c)
	)
)
(:action move_from_c_1
	:parameters ()
	:precondition (and
		(c)
	)
	:effect (and
		(not (c))
		(g)
	)
)
(:action move_from_c_2
	:parameters ()
	:precondition (and
		(c)
	)
	:effect (and
		(not (c))
		(g)
	)
)
(:action move_from_c_3
	:parameters ()
	:precondition (and
		(c)
	)
	:effect (and
		(not (c))
		(g)
	)
)

)