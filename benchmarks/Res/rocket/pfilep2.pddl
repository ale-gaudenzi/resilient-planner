(define (problem rocket-2)
  (:domain rockets)
  (:objects london paris jfk bos r1 r2 r3 r4 mxf  pencil
        fl0
        fl1
        fl2
        fl3
        fl4
        fl5
        fl6
 )
  (:init
        (next fl0 fl1)
        (next fl1 fl2)
        (next fl2 fl3)
        (next fl3 fl4)
        (next fl4 fl5)
        (next fl5 fl6)
        (flevel fl0)
        (flevel fl1)
        (flevel fl2)
        (flevel fl3)
        (flevel fl4)
        (flevel fl5)
        (flevel fl6)
        
        (fuel-level r1 fl6)
        (fuel-level r2 fl5)     
        (fuel-level r3 fl4)
        (fuel-level r4 fl3)
 
  	(place london) 
	 (place paris) 
	 (place jfk) 
	 (place bos)
	 
	 (rocket r1) 
	 (rocket r2) 
	 (rocket r3) 

	 (cargo mxf) 
	 (cargo pencil) 
	 
	 (at r1 jfk)
	 (at r2 london)
	 (at r3 london)
 
         (rocket r4)
         (at r4 bos)
 
	 (at mxf paris)
	 (at pencil london)
	 )
  (:goal (and 
	      (at mxf bos)
	      (at pencil bos)
))
)
