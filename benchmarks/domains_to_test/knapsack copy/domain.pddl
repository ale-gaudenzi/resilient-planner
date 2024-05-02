(define (domain knapsack)
  (:requirements :strips :negative-preconditions :typing)
  (:types knapsack agent items - object
	)

  (:predicates
    (is_close ?k - knapsack)
    (in ?o - object ?k - knapsack)
    (on_table ?o)
  )

  (:action open_0
    :parameters (?k - knapsack)
    :precondition (and (is_close ?k))
    :effect (not(is_close ?k))
  )
  (:action open_1
    :parameters (?k - knapsack)
    :precondition (and (is_close ?k))
    :effect (not(is_close ?k))
  )
  (:action open_2
    :parameters (?k - knapsack)
    :precondition (and (is_close ?k))
    :effect (not(is_close ?k))
  )



  (:action close_0
    :parameters (?k - knapsack)
    :precondition (and (not(is_close ?k)))
    :effect (is_close ?k)
  )
  (:action close_1
    :parameters (?k - knapsack)
    :precondition (and (not(is_close ?k)))
    :effect (is_close ?k)
  )
  ; (:action close_2
  ;   :parameters (?k - knapsack)
  ;   :precondition (and (not(is_close ?k)))
  ;   :effect (is_close ?k)
  ; )


  (:action put_in_knapsack_0
    :parameters (?k - knapsack ?o - items)
    :precondition (and(not(is_close ?k))(on_table ?o))
    :effect (and (in ?o ?k))
  )
  (:action put_in_knapsack_1
    :parameters (?k - knapsack ?o - items)
    :precondition (and(not(is_close ?k))(on_table ?o))
    :effect (and (in ?o ?k))
  )
  (:action put_in_knapsack_2
    :parameters (?k - knapsack ?o - items)
    :precondition (and(not(is_close ?k))(on_table ?o))
    :effect (and (in ?o ?k))
  )
  )