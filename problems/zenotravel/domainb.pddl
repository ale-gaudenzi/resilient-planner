(define (domain zeno-travel)
	(:predicates
		(at ?x ?c)
		(in ?p ?a)
		(aircraft ?p)
		(person ?x)
		(city ?x)
)
	(:action board
		:parameters ( ?p ?a ?c)
		:precondition (and (person ?p) (aircraft ?a) (city ?c) (at ?p ?c) (at ?a ?c))
		:effect (and (in ?p ?a) (not (at ?p ?c)))
	)

	(:action debark
		:parameters ( ?p ?a ?c)
		:precondition (and (person ?p) (aircraft ?a) (city ?c) (in ?p ?a) (at ?a ?c))
		:effect (and (at ?p ?c) (not (in ?p ?a)))
	)

	(:action fly
		:parameters ( ?a ?c1 ?c2)
		:precondition (and (aircraft ?a) (city ?c1) (city ?c2)  (at ?a ?c1) )
		:effect (and (at ?a ?c2) (not (at ?a ?c1)) )
	)

)
