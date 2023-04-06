(define (domain simple-travel)
	(:predicates
		(at ?x ?c)
		(aircraft ?p)
		(city ?x)
	)

	(:action fly
		:parameters ( ?a ?c1 ?c2)
		:precondition (and (aircraft ?a) (city ?c1) (city ?c2) (at ?a ?c1))
		:effect (and (at ?a ?c2) (not (at ?a ?c1)) )
	)
)