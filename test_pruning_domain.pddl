(define (domain blocks)
	(:requirements :typing)
(:types
	car location dummy - object
)

(:predicates
	(on ?c - object ?l - location)
	(connected ?l1 ?l2 - location)
	(activatee ?d - dummy)
)

(:action move
	:parameters (?c - car ?l1 ?l2 - location )
	:precondition (and
		(on ?c ?l1)
		(connected ?l1 ?l2)
	)
	:effect (and
		(not (on ?c ?l1))
		(on ?c ?l2)
	)
)


(:action doit
	:parameters (?c - car ?d - dummy ?l - location)
	:precondition(and
		(on ?c ?l)
		(on ?d ?l)
	)
	:effect (and
		(activatee ?d)
	)
)

)