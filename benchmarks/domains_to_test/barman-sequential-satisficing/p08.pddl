(define (problem prob)
 (:domain barman)
 (:objects 
     shaker1 shaker2 - shaker
     left left_b - hand_a
     right right_b - hand_b
     shot1 shot2 - shot
     ingredient3 ingredient4 - ingredient
     cocktail1 cocktail2 - cocktail
     dispenser2 dispenser1 dispenser3 dispenser4 - dispenser
     l0 l1 l2 l3 - level
)
 (:init 
  (ontable shaker1)
  (ontable shaker2)
  (ontable shot1)
  (ontable shot2)
  (dispenses dispenser1 ingredient3)
  (dispenses dispenser1 ingredient4)
  (dispenses dispenser2 ingredient4)
  (dispenses dispenser2 ingredient3)
  (clean shaker1)
  (clean shaker2)
  (empty shaker1)
  (empty shaker2)
  (clean shot1)
  (empty shot1)
  (clean shot2)
  (empty shot2)
  (handempty left)
  (handempty left_b)
  (handempty right)
  (handempty right_b)
  (shaker-empty-level shaker1 l0)
  (shaker-empty-level shaker2 l0)
  (shaker-level shaker1 l0)
  (shaker-level shaker2 l0)
  (next l0 l1)
  (next l1 l2)
  (cocktail-part1 cocktail1 ingredient3)
  (cocktail-part2 cocktail1 ingredient4)
  (cocktail-part1 cocktail2 ingredient3)
  (cocktail-part2 cocktail2 ingredient4)
)
 (:goal
  (and
     (contains shot1 cocktail1)
     (contains shot2 cocktail2)
)))
