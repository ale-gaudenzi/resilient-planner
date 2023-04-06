(define (domain zeno-travel)
	(:requirements :negative-preconditions :strips)
	(:predicates
		(at ?x ?c)
		(in ?p ?a)
		(fuel-level ?a ?l)
		(next ?l1 ?l2)
		(aircraft ?p)
		(person ?x)
		(city ?x)
		(flevel ?x)

		(board ?p ?a ?c)
		(debark ?p ?a ?c)
		(fly ?a ?c1 ?c2 ?l1 ?l2)
		(zoom ?a ?c1 ?c2 ?l1 ?l2)
		(refuel ?a ?c ?l ?l1)

		(faults_0)
		(faults_1)
		(faults_2)
	)

	(:action board-0
		:parameters ( ?p ?a ?c)
		:precondition (and (person ?p) (aircraft ?a) (city ?c) (at ?p ?c) (at ?a ?c) (not (board ?p ?a ?c))(faults_0))
		:effect (oneof
			(and (in ?p ?a) (not (at ?p ?c)))
			(and (faults_1) (not(faults_0)) (board ?p ?a ?c))
		)
	)

	(:action debark-0
		:parameters ( ?p ?a ?c)
		:precondition (and (person ?p) (aircraft ?a) (city ?c) (in ?p ?a) (at ?a ?c)
			(not (debark ?p ?a ?c))(faults_0)
		)
		:effect (oneof
			(and (at ?p ?c) (not (in ?p ?a)))
			(and (faults_1) (not(faults_0)) (debark ?p ?a ?c))
		)
	)

	(:action fly-0
		:parameters ( ?a ?c1 ?c2 ?l1 ?l2)
		:precondition (and (aircraft ?a) (city ?c1) (city ?c2) (flevel ?l1) (flevel ?l2) (at ?a ?c1) (fuel-level ?a ?l1) (next ?l2 ?l1)
			(not (fly ?a ?c1 ?c2 ?l1 ?l2))(faults_0)
		)
		:effect (oneof
			(and (at ?a ?c2) (fuel-level ?a ?l2) (not (at ?a ?c1)) (not (fuel-level ?a ?l1)))
			(and (faults_1) (not(faults_0)) (fly ?a ?c1 ?c2 ?l1 ?l2))
		)
	)

	(:action zoom-0
		:parameters ( ?a ?c1 ?c2 ?l1 ?l2 ?l3)
		:precondition (and (aircraft ?a) (city ?c1) (city ?c2) (flevel ?l1) (flevel ?l2) (flevel ?l3) (at ?a ?c1) (fuel-level ?a ?l1) (next ?l2 ?l1) (next ?l3 ?l2)
			(not (zoom ?a ?c1 ?c2 ?l1 ?l2))(faults_0)
		)
		:effect (oneof
			(and (at ?a ?c2) (fuel-level ?a ?l3) (not (at ?a ?c1)) (not (fuel-level ?a ?l1)))
			(and (faults_1) (not(faults_0)) (zoom ?a ?c1 ?c2 ?l1 ?l2))
		)
	)

	(:action refuel-0
		:parameters ( ?a ?c ?l ?l1)
		:precondition (and (aircraft?a) (city ?c) (flevel ?l) (flevel ?l1) (fuel-level ?a ?l) (next ?l ?l1) (at ?a ?c)
			(not (refuel ?a ?c ?l ?l1))(faults_0)
		)
		:effect (oneof
			(and (fuel-level ?a ?l1) (not (fuel-level ?a ?l)))
			(and (faults_1) (not(faults_0)) (refuel ?a ?c ?l ?l1))
		)
	)

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	(:action board-1
		:parameters ( ?p ?a ?c)
		:precondition (and (person ?p) (aircraft ?a) (city ?c) (at ?p ?c) (at ?a ?c) (not (board ?p ?a ?c))(faults_1))
		:effect (oneof
			(and (in ?p ?a) (not (at ?p ?c)))
			(and (faults_2) (not(faults_1)) (board ?p ?a ?c))
		)
	)

	(:action debark-1
		:parameters ( ?p ?a ?c)
		:precondition (and (person ?p) (aircraft ?a) (city ?c) (in ?p ?a) (at ?a ?c)
			(not (debark ?p ?a ?c))(faults_1)
		)
		:effect (oneof
			(and (at ?p ?c) (not (in ?p ?a)))
			(and (faults_2) (not(faults_1)) (debark ?p ?a ?c))
		)
	)

	(:action fly-1
		:parameters ( ?a ?c1 ?c2 ?l1 ?l2)
		:precondition (and (aircraft ?a) (city ?c1) (city ?c2) (flevel ?l1) (flevel ?l2) (at ?a ?c1) (fuel-level ?a ?l1) (next ?l2 ?l1)
			(not (fly ?a ?c1 ?c2 ?l1 ?l2))(faults_1)
		)
		:effect (oneof
			(and (at ?a ?c2) (fuel-level ?a ?l2) (not (at ?a ?c1)) (not (fuel-level ?a ?l1)))
			(and (faults_2) (not(faults_1)) (fly ?a ?c1 ?c2 ?l1 ?l2))
		)
	)

	(:action zoom-1
		:parameters ( ?a ?c1 ?c2 ?l1 ?l2 ?l3)
		:precondition (and (aircraft ?a) (city ?c1) (city ?c2) (flevel ?l1) (flevel ?l2) (flevel ?l3) (at ?a ?c1) (fuel-level ?a ?l1) (next ?l2 ?l1) (next ?l3 ?l2)
			(not (zoom ?a ?c1 ?c2 ?l1 ?l2))(faults_1)
		)
		:effect (oneof
			(and (at ?a ?c2) (fuel-level ?a ?l3) (not (at ?a ?c1)) (not (fuel-level ?a ?l1)))
			(and (faults_2) (not(faults_1)) (zoom ?a ?c1 ?c2 ?l1 ?l2))
		)
	)

	(:action refuel-1
		:parameters ( ?a ?c ?l ?l1)
		:precondition (and (aircraft?a) (city ?c) (flevel ?l) (flevel ?l1) (fuel-level ?a ?l) (next ?l ?l1) (at ?a ?c)
			(not (refuel ?a ?c ?l ?l1))(faults_1)
		)
		:effect (oneof
			(and (fuel-level ?a ?l1) (not (fuel-level ?a ?l)))
			(and (faults_2) (not(faults_1)) (refuel ?a ?c ?l ?l1))
		)
	)

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	(:action board-2
		:parameters ( ?p ?a ?c)
		:precondition (and (person ?p) (aircraft ?a) (city ?c) (at ?p ?c) (at ?a ?c)
			(not (board ?p ?a ?c)) (faults_2))
		:effect (and (in ?p ?a) (not (at ?p ?c)))
	)

	(:action debark-2
		:parameters ( ?p ?a ?c)
		:precondition (and (person ?p) (aircraft ?a) (city ?c) (in ?p ?a) (at ?a ?c)
			(not (debark ?p ?a ?c)) (faults_2)
		)
		:effect (and (at ?p ?c) (not (in ?p ?a)))
	)

	(:action fly-2
		:parameters ( ?a ?c1 ?c2 ?l1 ?l2)
		:precondition (and (aircraft ?a) (city ?c1) (city ?c2) (flevel ?l1) (flevel ?l2) (at ?a ?c1) (fuel-level ?a ?l1) (next ?l2 ?l1)
			(not (fly ?a ?c1 ?c2 ?l1 ?l2)) (faults_2)
		)
		:effect (and (at ?a ?c2) (fuel-level ?a ?l2) (not (at ?a ?c1)) (not (fuel-level ?a ?l1)))
	)

	(:action zoom-2
		:parameters ( ?a ?c1 ?c2 ?l1 ?l2 ?l3)
		:precondition (and (aircraft ?a) (city ?c1) (city ?c2) (flevel ?l1) (flevel ?l2) (flevel ?l3) (at ?a ?c1) (fuel-level ?a ?l1) (next ?l2 ?l1) (next ?l3 ?l2)
			(not (zoom ?a ?c1 ?c2 ?l1 ?l2)) (faults_2)
		)
		:effect (and (at ?a ?c2) (fuel-level ?a ?l3) (not (at ?a ?c1)) (not (fuel-level ?a ?l1)))
	)

	(:action refuel-2
		:parameters ( ?a ?c ?l ?l1)
		:precondition (and (aircraft?a) (city ?c) (flevel ?l) (flevel ?l1) (fuel-level ?a ?l) (next ?l ?l1) (at ?a ?c)
			(not (refuel ?a ?c ?l ?l1)) (faults_2)
		)
		:effect (and (fuel-level ?a ?l1) (not (fuel-level ?a ?l)))
	)

)