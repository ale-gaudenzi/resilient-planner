(define (problem logistics-9-0) (:domain logistics)
(:objects
	tru1 - truck
	tru2 - truck
	tru3 - truck
	tru4 - truck
	tru5 - truck
	tru6 - truck
	
	pos1 - location
	pos2 - location
	pos3 - location
	pos4 - location
	pos5 - location
	pos6 - location

	apn1 - airplane
	apn2 - airplane
	apn3 - airplane
	apn4 - airplane	
	apn5 - airplane	
	apn6 - airplane	
	
	apt1 - airport
	apt2 - airport
	apt3 - airport
	apt4 - airport
	apt5 - airport
	apt6 - airport

	cit1 - city
	cit2 - city
	cit3 - city
	cit4 - city
	cit5 - city
	cit6 - city

	obj1 - package
)
(:init
	(at obj1 pos1)

	(at apn1 apt2)
	(at apn2 apt2)	
	(at apn3 apt2)
	(at apn4 apt2)
	(at apn5 apt3)
	(at apn6 apt3)	

	(at tru1 pos1)
	(at tru2 pos2)
	(at tru3 pos3)
	(at tru4 pos4)
	(at tru5 pos1)
	(at tru6 pos2)


	(in-city tru1 pos1 cit1)
	(in-city tru1 apt1 cit1)

	(in-city tru2 pos2 cit2)
	(in-city tru2 apt2 cit2)

	(in-city tru3 pos3 cit3)
	(in-city tru3 apt3 cit3)

	(in-city tru4 pos4 cit4)
	(in-city tru4 apt4 cit4)

	(in-city tru5 pos1 cit1)
	(in-city tru5 apt1 cit1)

	(in-city tru6 pos2 cit2)
	(in-city tru6 apt2 cit2)

)
(:goal
	(and
		(at obj1 pos4)
	)
)
)
