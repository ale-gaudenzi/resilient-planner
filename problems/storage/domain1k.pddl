; IPC5 Domain: Storage Propositional
; Authors: Alfonso Gerevini and Alessandro Saetti 

(define (domain Storage-Propositional)
	(:requirements :typing :negative-preconditions)
	(:types
		hoist surface place area - object
		container depot - place
		storearea transitarea - area
		area crate - surface
	)

	(:predicates
		(clear ?s - storearea)
		(in ?x -
			(either storearea crate) ?p - place)
		(available ?h - hoist)
		(lifting ?h - hoist ?c - crate)
		(at ?h - hoist ?a - area)
		(on ?c - crate ?s - storearea)
		(connected ?a1 ?a2 - area)
		(compatible ?c1 ?c2 - crate)

		(lift ?h - hoist ?c - crate ?a1 - storearea ?a2 - area ?p - place)
		(drop ?h - hoist ?c - crate ?a1 - storearea ?a2 - area ?p - place)
		(move ?h - hoist ?from ?to - storearea)
		(go-out ?h - hoist ?from - storearea ?to - transitarea)
		(go-in ?h - hoist ?from - transitarea ?to - storearea)

		(faults_0)
		(faults_1)
	)

	(:action lift-0
		:parameters (?h - hoist ?c - crate ?a1 - storearea ?a2 - area ?p - place)
		:precondition (and
			(connected ?a1 ?a2)
			(at ?h ?a2)
			(available ?h)
			(on ?c ?a1)
			(in ?a1 ?p)
			(not (lift ?h ?c ?a1 ?a2 ?p))
			(faults_0)
		)
		:effect (oneof
			(and (clear ?a1)
				(not (available ?h))
				(lifting ?h ?c)
				(not (in ?c ?p))
				(not (on ?c ?a1)))
			(and(faults_1)
				(not(faults_0))(lift ?h ?c ?a1 ?a2 ?p))
		)
	)

	(:action drop-0
		:parameters (?h - hoist ?c - crate ?a1 - storearea ?a2 - area ?p - place)
		:precondition (and (connected ?a1 ?a2) (at ?h ?a2) (lifting ?h ?c)
			(clear ?a1) (in ?a1 ?p) (not(drop ?h ?c ?a1 ?a2 ?p)) (faults_0)
		)
		:effect (oneof
			(and (not (lifting ?h ?c)) (available ?h)
				(not (clear ?a1)) (on ?c ?a1) (in ?c ?p))
			(and(faults_1)
				(not(faults_0))(drop ?h ?c ?a1 ?a2 ?p))
		)
	)

	(:action move-0
		:parameters (?h - hoist ?from ?to - storearea)
		:precondition (and (faults_0)
			(at ?h ?from) (clear ?to) (connected ?from ?to) (not(move ?h ?from ?to)))
		:effect (oneof
			(and (not (at ?h ?from)) (at ?h ?to) (not (clear ?to)) (clear ?from))
			(and(faults_1)
				(not(faults_0))(move ?h ?from ?to))
		)
	)

	(:action go-out-0
		:parameters (?h - hoist ?from - storearea ?to - transitarea)
		:precondition (and (faults_0)
			(at ?h ?from) (connected ?from ?to) (not(go-out ?h ?from ?to)))
		:effect (oneof
			(and (not (at ?h ?from)) (at ?h ?to) (clear ?from))
			(and(faults_1)
				(not(faults_0))(go-out ?h ?from ?to))
		)
	)

	(:action go-in-0
		:parameters (?h - hoist ?from - transitarea ?to - storearea)
		:precondition (and (faults_0)
			(at ?h ?from) (connected ?from ?to) (clear ?to)
			(not(go-in ?h ?from ?to)))
		:effect (oneof
			(and (not (at ?h ?from)) (at ?h ?to) (not (clear ?to)))
			(and(faults_1)
				(not(faults_0))(go-in ?h ?from ?to))
		)
	)

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	(:action lift-1
		:parameters (?h - hoist ?c - crate ?a1 - storearea ?a2 - area ?p - place)
		:precondition (and (faults_1)

			(connected ?a1 ?a2)
			(at ?h ?a2)
			(available ?h)
			(on ?c ?a1)
			(in ?a1 ?p)
			(not (lift ?h ?c ?a1 ?a2 ?p)))
		:effect (and (clear ?a1)
			(not (available ?h))
			(lifting ?h ?c)
			(not (in ?c ?p))
			(not (on ?c ?a1)))
	)

	(:action drop-1
		:parameters (?h - hoist ?c - crate ?a1 - storearea ?a2 - area ?p - place)
		:precondition (and (faults_1)(connected ?a1 ?a2) (at ?h ?a2) (lifting ?h ?c)
			(clear ?a1) (in ?a1 ?p) (not(drop ?h ?c ?a1 ?a2 ?p)))
		:effect (and (not (lifting ?h ?c)) (available ?h)
			(not (clear ?a1)) (on ?c ?a1) (in ?c ?p))
	)

	(:action move-1
		:parameters (?h - hoist ?from ?to - storearea)
		:precondition (and (faults_1)
			(at ?h ?from) (clear ?to) (connected ?from ?to) (not(move ?h ?from ?to)))
		:effect (and (not (at ?h ?from)) (at ?h ?to) (not (clear ?to)) (clear ?from))
	)

	(:action go-out-1
		:parameters (?h - hoist ?from - storearea ?to - transitarea)
		:precondition (and(faults_1) (at ?h ?from) (connected ?from ?to) (not(go-out ?h ?from ?to)))
		:effect (and (not (at ?h ?from)) (at ?h ?to) (clear ?from))
	)

	(:action go-in-1
		:parameters (?h - hoist ?from - transitarea ?to - storearea)
		:precondition (and (faults_1)
			(at ?h ?from) (connected ?from ?to) (clear ?to)
			(not(go-in ?h ?from ?to)))
		:effect (and (not (at ?h ?from)) (at ?h ?to) (not (clear ?to)))
	)
)