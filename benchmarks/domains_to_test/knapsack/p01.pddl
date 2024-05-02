(define (problem knapsack_1)
   (:domain knapsack)
   (:objects
      a0 a1 - agent
      pencil - items
      k0 - knapsack
   )

   (:init
      (is_close k0)
      (able_to_open k0 a0)
      (able_to_open k0 a1)
      (able_to_close k0 a0)
   )

   (:goal
      (and
         (in pencil k0)
         (is_close k0)
      )
   )
)