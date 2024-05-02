; child-snack task with 10 children and 0.4 gluten factor 
; constant factor of 1.3
; random seed: 234324

(define (problem prob-snack)
  (:domain child-snack)
  (:objects
    child1 - child
    bread1 bread2 bread3 bread4 - bread-portion
    content1 content2 content3 content4 content5 content6 - content-portion
    tray1 tray2 tray3 - tray
    table1 table2 - place
    sandw1 sandw2 - sandwich
  )
  (:init
     (at tray1 kitchen)
     (at tray2 kitchen)
     (at tray3 kitchen)
     (at_kitchen_bread bread1)
     (at_kitchen_bread bread2)
     (at_kitchen_bread bread3)
     (at_kitchen_bread bread4)
     (at_kitchen_content content1)
     (at_kitchen_content content2)
     (at_kitchen_content content3)
     (at_kitchen_content content4)
     (at_kitchen_content content5)
     (at_kitchen_content content6)
     (no_gluten_bread bread2)
     (no_gluten_content content1)
     (allergic_gluten child1)
     (waiting child1 table2)
     (notexist sandw1)
     (notexist sandw2)
  )
  (:goal
    (and
     (served child1)
    )
  )
)
