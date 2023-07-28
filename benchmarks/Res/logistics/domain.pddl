(define (domain logistics)
	(:requirements :typing)
(:types
	location vehicle package city - object
	airport - location
	truck airplane - vehicle
)
(:predicates
	(at ?obj - object ?loc - location)
	(in ?obj1 - package ?veh - vehicle)
	(in-city ?agent - truck ?loc - location ?city - city)
)

(:action load-airplane
	:parameters (?airplane - airplane ?obj - package ?loc - airport)
	:precondition (and
		(at ?obj ?loc)
		(at ?airplane ?loc)
	)
	:effect (and
		(not (at ?obj ?loc))
		(in ?obj ?airplane)
	)
)


(:action unload-airplane
	:parameters (?airplane - airplane ?obj - package ?loc - airport)
	:precondition (and
		(in ?obj ?airplane)
		(at ?airplane ?loc)
	)
	:effect (and
		(not (in ?obj ?airplane))
		(at ?obj ?loc)
	)
)


(:action fly-airplane
	:parameters (?airplane - airplane ?loc-from - airport ?loc-to - airport)
	:precondition 
		(at ?airplane ?loc-from)
	:effect (and
		(not (at ?airplane ?loc-from))
		(at ?airplane ?loc-to)
	)
)


(:action load-truck
	:parameters (?truck - truck ?obj - package ?loc - location)
	:precondition (and
		(at ?truck ?loc)
		(at ?obj ?loc)
	)
	:effect (and
		(not (at ?obj ?loc))
		(in ?obj ?truck)
	)
)


(:action unload-truck
	:parameters (?truck - truck ?obj - package ?loc - location)
	:precondition (and
		(at ?truck ?loc)
		(in ?obj ?truck)
	)
	:effect (and
		(not (in ?obj ?truck))
		(at ?obj ?loc)
	)
)


(:action drive-truck
	:parameters (?truck - truck ?loc-from - location ?loc-to - location ?city - city)
	:precondition (and
		(at ?truck ?loc-from)
		(in-city ?truck ?loc-from ?city)
		(in-city ?truck ?loc-to ?city)
	)
	:effect (and
		(not (at ?truck ?loc-from))
		(at ?truck ?loc-to)
	)
)

)