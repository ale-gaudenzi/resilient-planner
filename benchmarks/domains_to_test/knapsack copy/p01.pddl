(define (problem knapsack_1)
   (:domain knapsack)
   (:objects
      pencil - items
      k0 - knapsack
   )

   (:init
      (is_close k0)
      (on_table pencil)
   )

   (:goal
      (and
         (in pencil k0)
         (is_close k0)
      )
   )
)