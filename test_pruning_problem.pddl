(define (problem BLOCKS-4-0) (:domain blocks)
(:objects
	car1 - car
	car2 - car
	car3 - car
	car4 - car
	car5 - car
	phone - dummy
	l1 - location
	l2 - location
	l3 - location
	l4 - location
)
(:init
	(on car1 l1)
	(on car2 l1)
	(on car3 l1)
	(on car4 l1)
	(on car5 l1)
	(connected l1 l2)
	(connected l2 l3)
	(connected l3 l4)
	(on phone l4)
)
(:goal
	(and
		(activatee phone)
	)
)
)
