; Rocket domain used in (Nguyen & Kambhampati 2001) + Fuel levels

(define (domain rockets)
  (:predicates (cargo ?object)
	       (rocket ?rocket)
	       (place ?place)
	       (at ?object ?place)
	       (in ?object ?rocket)
	       (flevel ?l)
	       (fuel-level ?a ?l) (next ?l1 ?l2)
)
  (:action load
	   :parameters (?object ?rocket ?place)
	   :precondition (and (cargo ?object) (rocket ?rocket) (place ?place)
			      (at ?rocket ?place) (at ?object ?place))
	   :effect (and (in ?object ?rocket) (not (at ?object ?place))))
  (:action unload
	   :parameters (?object ?rocket ?place)
	   :precondition (and (cargo ?object) (rocket ?rocket) (place ?place)
			      (at ?rocket ?place) (in ?object ?rocket))
	   :effect (and (at ?object ?place) (not (in ?object ?rocket))))
  (:action move
	   :parameters (?rocket ?from ?to ?l1 ?l2)
	   :precondition (and (rocket ?rocket) (place ?from) (place ?to)
			      (at ?rocket ?from)
                              (flevel ?l1) (flevel ?l2)
			      (fuel-level ?rocket ?l1) (next ?l2 ?l1)
                         )
	   :effect (and (at ?rocket ?to) 
			(not (at ?rocket ?from))
			(fuel-level ?rocket ?l2)
			(not (fuel-level ?rocket ?l1))
  		    ))
  (:action refuel
   :parameters ( ?rocket ?l ?l1)
   :precondition
	     (and (rocket ?rocket) (flevel ?l) (flevel ?l1)  (fuel-level ?rocket ?l) (next ?l ?l1) )
    :effect
        (and (fuel-level ?rocket ?l1) (not (fuel-level ?rocket ?l))))

)




