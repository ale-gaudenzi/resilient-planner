(define (domain blocks)
	(:predicates
		(a)
		(b)
		(c)
		(g)
		(x)
		(failure)
	)

	(:action m_1
		:parameters ()
		:precondition (and
			(a)
		)
		:effect (and (not(a))(b))
	)
	(:action m_2
		:parameters ()
		:precondition (and
			(b)
		)
		:effect (oneof
			(and (not(b))(g))
			(and (failure))
		)
	)
	(:action m_3
		:parameters ()
		:precondition (and
			(b)
		)
		:effect (oneof
			(and (not(b))(c))
			(and (failure))
		)
	)
	(:action m_4
		:parameters ()
		:precondition (and
			(c)
		)
		:effect (oneof
			(and (not(c))(g))
			(and (failure))
		)
	)

	(:action m_5
		:parameters ()
		:precondition (and
			(g)
		)
		:effect (and (not(g))(x))
	)
)