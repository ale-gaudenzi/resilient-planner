(define (problem prob)
 (:domain barman)
 (:objects 
     shaker1 - shaker
     left left_b - hand_a
     right right_b - hand_b
     shot1 shot2 - shot
     ingredient3 ingredient4 - ingredient
     cocktail1 - cocktail
     dispenser2 dispenser1 dispenser3 dispenser4 - dispenser
     l0 l1 l2 - level
)
 (:init 
  (ontable shaker1)
  (ontable shot1)
  (ontable shot2)
  (dispenses dispenser3 ingredient3)
  (dispenses dispenser4 ingredient4)
  (clean shaker1)
  (clean shot1)
  (clean shot2)
  (empty shaker1)
  (empty shot1)
  (empty shot2)
  (handempty left)
  (handempty left_b)
  (handempty right)
  (handempty right_b)
  (shaker-empty-level shaker1 l0)
  (shaker-level shaker1 l0)
  (next l0 l1)
  (next l1 l2)
  (cocktail-part1 cocktail1 ingredient3)
  (cocktail-part2 cocktail1 ingredient4)
)
 (:goal
  (and
     (contains shot1 cocktail1)
)))
