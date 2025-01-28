(define (domain blocks)
(:predicates
	(a)
	(b)
	(c)
)

(:action move_from_i_a
	:parameters ()
	:precondition (and
		(i)
	)
	:effect (and
		(not (i))
		(a)
	)
)
(:action move_from_i_b
	:parameters ()
	:precondition (and
		(i)
	)
	:effect (and
		(not (i))
		(b)
	)
)

(:action move_from_a_c_1
	:parameters ()
	:precondition (and
		(a)
	)
	:effect (and
		(not (a))
		(c)
	)
)
(:action move_from_a_c_2
	:parameters ()
	:precondition (and
		(a)
	)
	:effect (and
		(not (a))
		(c)
	)
)
(:action move_from_b_c_1
	:parameters ()
	:precondition (and
		(b)
	)
	:effect (and
		(not (b))
		(c)
	)
)
(:action move_from_b_c_2
	:parameters ()
	:precondition (and
		(b)
	)
	:effect (and
		(not (b))
		(c)
	)
)
(:action move_from_c_d_1
	:parameters ()
	:precondition (and
		(c)
	)
	:effect (and
		(not (c))
		(d)
	)
)
(:action move_from_c_d_2
	:parameters ()
	:precondition (and
		(c)
	)
	:effect (and
		(not (c))
		(d)
	)
)
(:action move_from_d_g_1
	:parameters ()
	:precondition (and
		(d)
	)
	:effect (and
		(not (d))
		(g)
	)
)
(:action move_from_d_g_2
	:parameters ()
	:precondition (and
		(d)
	)
	:effect (and
		(not (d))
		(g)
	)
)
)