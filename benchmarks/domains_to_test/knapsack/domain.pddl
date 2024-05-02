(define (domain knapsack)
  (:requirements :strips :negative-preconditions :typing)
  (:types knapsack agent items - object
	)

  (:predicates
    (is_close ?k - knapsack)
    (holding ?a - agent)
    (is_hold ?o - items ?a - agent)
    (in ?o - object ?k - knapsack)
    (able_to_open ?k - knapsack  ?a - agent)
    (able_to_close ?k - knapsack ?a - agent)
  )

  (:action open
    :parameters (?k - knapsack ?a - agent)
    :precondition (and (is_close ?k)(not(holding ?a))(able_to_open ?k ?a))
    :effect (not(is_close ?k))
  )

  (:action close
    :parameters (?k - knapsack ?a - agent)
    :precondition (and (not(is_close ?k)) (not(holding ?a))(able_to_close ?k ?a))
    :effect (is_close ?k)
  )

  (:action hold
    :parameters (?a - agent ?o - items)
    :precondition (and (not(holding ?a)))
    :effect (and
      (is_hold ?o ?a)
      (holding ?a)
    )
  )

  (:action put_in_knapsack
    :parameters (?k - knapsack ?a - agent ?o - items)
    :precondition (and (is_hold ?o ?a) (not(is_close ?k)))
    :effect (and (in ?o ?k) (not(holding ?a))))
  )