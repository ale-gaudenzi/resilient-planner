(define (problem rocket-ext-a)
  (:domain rockets)
  (:objects london paris jfk bos rome tokyo  r1 r2  r3 r4 r5 r6 mxf avrim  
        fl0
        fl1
        fl2
        fl3
        fl4
        fl5
        fl6 )
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
        (fuel-level r5 fl4)
        (fuel-level r6 fl5)
    
         (place london) 
	 (place paris) 
	 (place jfk) 
	 (place bos)
	 (place rome)
	 (place tokyo)
	 
	 (rocket r1) 
	 (rocket r2) 
	 (rocket r3) 
	 (rocket r4) 
	 (rocket r5) 
	 (rocket r6) 

	 (cargo mxf) 
	 (cargo avrim) 
	 
	 (at r1 jfk)
	 (at r2 london)
	 (at r3 london)
	 (at r4 paris)
	 (at r5 rome)
	 (at r6 tokyo)
	 
	 (at mxf paris)
	 (at avrim paris)
	 )
  (:goal (and 
	      (at mxf bos)
              (at avrim jfk)
))
)
