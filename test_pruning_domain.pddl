(define (domain blocks)
(:predicates
    (start)
    (a)
    (b)
    (c)
    (goal)
)
(:action move_from_init_to_b
	:parameters ()
	:precondition (and
		(start)
	)
	:effect (and
		(b)(goal)
	)
)
(:action move_from_init_to_a
	:parameters ()
	:precondition (and
		(start)
	)
	:effect (and
		(a)(goal)
	)
)

(:action move_from_init_to_c
	:parameters ()
	:precondition (and
		(start)
	)
	:effect (and
		(c)(goal)
	)
)

(:action move_from_a_to_goal
	:parameters ()
	:precondition 
		(and (a)(goal))
	:effect (and
		(not (start))
	)
)

(:action del_par
	:parameters ()
	:precondition (and )
	:effect (and
		(not (a))
		(not(b))
		(not(c))
	)
)


(:action move_from_b_to_goal
	:parameters ()
	:precondition 
		(and (b)(goal))
	:effect (and
		(not (start))
	)
)

(:action move_from_c_to_goal
	:parameters ()
	:precondition(and
		(c)(goal)
    )
	:effect (and
		(not (start))
	)
)
)