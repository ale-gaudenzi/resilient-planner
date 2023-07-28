(define (problem logistics-9-0) (:domain logistics)
(:objects
	tru1_1 - truck
	tru2_1 - truck
	tru3_1 - truck
	tru4_1 - truck
	tru1_2 - truck
	tru2_2 - truck
	tru3_2 - truck
	tru4_2 - truck
	tru1_3 - truck
	tru2_3 - truck
	tru3_3 - truck
	tru4_3 - truck
	tru1_4 - truck
	tru2_4 - truck
	tru3_4 - truck
	tru4_4 - truck
	tru1_5 - truck
	tru2_5 - truck
	tru3_5 - truck
	tru4_5 - truck
	
	pos1_1 - location
	pos2_1 - location
	pos3_1 - location
	pos4_1 - location
	pos1_2 - location
	pos2_2 - location
	pos3_2 - location
	pos4_2 - location
	pos1_3 - location
	pos2_3 - location
	pos3_3 - location
	pos4_3 - location
	pos1_4 - location
	pos2_4 - location
	pos3_4 - location
	pos4_4 - location
	pos1_5 - location
	pos2_5 - location
	pos3_5 - location
	pos4_5 - location

	apn1 - airplane
	apn2 - airplane
	apn3 - airplane
	apn4 - airplane			
	apn5 - airplane
	
	obj21 - package
	obj22 - package
	obj23 - package

	apt4 - airport
	apt3 - airport
	apt2 - airport
	apt1 - airport
	obj33 - package
	obj32 - package
	obj31 - package
	cit1 - city
	cit2 - city
	cit3 - city
	cit4 - city
	obj11 - package
	obj13 - package
	obj12 - package
)
(:init
	(at apn1 apt2)
	(at apn2 apt2)	
	(at apn3 apt2)
	(at apn4 apt2)
	(at apn5 apt2)
	
	(at tru1_1 pos1_1)
	(at tru1_2 pos1_2)
	(at tru1_3 pos1_3)
	(at tru1_4 pos1_4)
	(at tru1_5 pos1_5)


	(at obj11 pos1_1)
	(at obj12 pos1_2)
	(at obj13 pos1_3)
	
	(at tru2_1 pos2_1)
	(at tru2_2 pos2_2)
	(at tru2_3 pos2_3)
	(at tru2_4 pos2_4)
	(at tru2_5 pos2_5)
	
	(at obj21 pos2_1)
	(at obj22 pos2_2)
	(at obj23 pos2_3)

	(at tru3_1 pos3_1)
	(at tru3_2 pos3_2)
	(at tru3_3 pos3_3)
	(at tru3_4 pos3_4)
	(at tru3_5 pos3_5)


	(at tru4_1 pos4_1)
	(at tru4_2 pos4_2)
	(at tru4_3 pos4_3)
	(at tru4_4 pos4_4)
	(at tru4_5 pos4_5)

	(at obj31 pos3_1)
	(at obj32 pos3_2)
	(at obj33 pos3_3)
	
	(in-city tru1_1 pos1_1 cit1)
	(in-city tru1_1 apt1 cit1)
	(in-city tru1_1 pos1_2 cit1)
	(in-city tru1_1 pos1_3 cit1)
	(in-city tru1_1 pos1_4 cit1)	
	(in-city tru1_1 pos1_5 cit1)
	
	(in-city tru1_2 pos1_2 cit1)
	(in-city tru1_2 apt1 cit1)
	(in-city tru1_2 pos1_1 cit1)
	(in-city tru1_2 pos1_3 cit1)
	(in-city tru1_2 pos1_4 cit1)	
	(in-city tru1_2 pos1_5 cit1)

	(in-city tru1_3 pos1_3 cit1)
	(in-city tru1_3 apt1 cit1)
	(in-city tru1_3 pos1_2 cit1)
	(in-city tru1_3 pos1_1 cit1)
	(in-city tru1_3 pos1_4 cit1)	
	(in-city tru1_3 pos1_5 cit1)
	
	(in-city tru1_4 pos1_1 cit1)
	(in-city tru1_4 apt1 cit1)
	(in-city tru1_4 pos1_2 cit1)
	(in-city tru1_4 pos1_3 cit1)
	(in-city tru1_4 pos1_4 cit1)	
	(in-city tru1_4 pos1_5 cit1)

	(in-city tru1_5 pos1_1 cit1)
	(in-city tru1_5 apt1 cit1)
	(in-city tru1_5 pos1_2 cit1)
	(in-city tru1_5 pos1_3 cit1)
	(in-city tru1_5 pos1_4 cit1)	
	(in-city tru1_5 pos1_5 cit1)
	
	(in-city tru2_1 pos2_1 cit2)
	(in-city tru2_1 apt2 cit2)
	(in-city tru2_1 pos2_2 cit2)
	(in-city tru2_1 pos2_3 cit1)
	(in-city tru2_1 pos2_4 cit1)
	(in-city tru2_1 pos2_5 cit1)

	(in-city tru2_2 pos2_1 cit2)
	(in-city tru2_2 apt2 cit2)
	(in-city tru2_2 pos2_2 cit2)
	(in-city tru2_2 pos2_3 cit2)	
	(in-city tru2_2 pos2_4 cit2)
	(in-city tru2_2 pos2_5 cit2)
	
	(in-city tru2_3 pos2_1 cit2)
	(in-city tru2_3 apt2 cit2)
	(in-city tru2_3 pos2_2 cit2)
	(in-city tru2_3 pos2_3 cit2)
	(in-city tru2_3 pos2_4 cit2)
	(in-city tru2_3 pos2_5 cit2)	

	(in-city tru2_4 pos2_1 cit2)
	(in-city tru2_4 apt2 cit2)
	(in-city tru2_4 pos2_2 cit2)
	(in-city tru2_4 pos2_3 cit2)
	(in-city tru2_4 pos2_4 cit2)
	(in-city tru2_4 pos2_5 cit2)
	
	(in-city tru2_5 pos2_1 cit2)
	(in-city tru2_5 apt2 cit2)
	(in-city tru2_5 pos2_2 cit2)
	(in-city tru2_5 pos2_3 cit2)
	(in-city tru2_5 pos2_4 cit2)
	(in-city tru2_5 pos2_5 cit2)

	(in-city tru3_1 pos3_1 cit3)
	(in-city tru3_1 apt3 cit3)
	(in-city tru3_1 pos3_2 cit3)
	(in-city tru3_1 pos3_3 cit3)
	(in-city tru3_1 pos3_4 cit3)
	(in-city tru3_1 pos3_5 cit3)
	
	(in-city tru3_2 pos3_1 cit3)	
	(in-city tru3_2 apt3 cit3)
	(in-city tru3_2 pos3_2 cit3)
	(in-city tru3_2 pos3_3 cit3)
	(in-city tru3_2 pos3_4 cit3)
	(in-city tru3_2 pos3_5 cit3)


	(in-city tru3_3 pos3_1 cit3)
	(in-city tru3_3 apt3 cit3)
	(in-city tru3_3 pos3_2 cit3)
	(in-city tru3_3 pos3_3 cit3)
	(in-city tru3_3 pos3_4 cit3)
	(in-city tru3_3 pos3_5 cit3)
	
	(in-city tru3_4 pos3_1 cit3)
	(in-city tru3_4 apt3 cit3)
	(in-city tru3_4 pos3_2 cit3)
	(in-city tru3_4 pos3_3 cit3)
	(in-city tru3_4 pos3_4 cit3)
	(in-city tru3_4 pos3_5 cit3)
	
	(in-city tru3_5 pos3_1 cit3)
	(in-city tru3_5 apt3 cit3)
	(in-city tru3_5 pos3_2 cit3)
	(in-city tru3_5 pos3_3 cit3)
	(in-city tru3_5 pos3_4 cit3)
	(in-city tru3_5 pos3_5 cit3)


	(in-city tru4_1 pos4_1 cit4)
	(in-city tru4_1 apt4 cit4)
	(in-city tru4_1 pos4_2 cit4)
	(in-city tru4_1 pos4_3 cit4)
	(in-city tru4_1 pos4_4 cit4)
	(in-city tru4_1 pos4_5 cit4)
	
	(in-city tru4_2 pos4_1 cit4)	
	(in-city tru4_2 apt4 cit4)
	(in-city tru4_2 pos4_2 cit4)
	(in-city tru4_2 pos4_3 cit4)
	(in-city tru4_2 pos4_4 cit4)
	(in-city tru4_2 pos4_5 cit4)


	(in-city tru4_3 pos4_1 cit4)
	(in-city tru4_3 apt4 cit4)
	(in-city tru4_3 pos4_2 cit4)
	(in-city tru4_3 pos4_3 cit4)
	(in-city tru4_3 pos4_4 cit4)
	(in-city tru4_3 pos4_5 cit4)
	
	(in-city tru4_4 pos4_1 cit4)
	(in-city tru4_4 apt4 cit4)
	(in-city tru4_4 pos4_2 cit4)
	(in-city tru4_4 pos4_3 cit4)
	(in-city tru4_4 pos4_4 cit4)
	(in-city tru4_4 pos4_5 cit4)
	
	(in-city tru4_5 pos4_1 cit4)
	(in-city tru4_5 apt4 cit4)
	(in-city tru4_5 pos4_2 cit4)
	(in-city tru4_5 pos4_3 cit4)
	(in-city tru4_5 pos4_4 cit4)
	(in-city tru4_5 pos4_5 cit4)


)
(:goal
	(and
		(at obj11 pos1_2)
		(at obj23 pos2_1)
		(at obj33 pos3_4)
		(at obj32 pos1_2)
		(at obj22 pos1_2)
		(at obj31 apt3)
		(at obj13 apt3)
		(at obj12 pos2_2)
		(at obj21 apt1)
	)
)
)
