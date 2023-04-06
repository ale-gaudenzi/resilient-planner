(define (domain grid)
        (:requirements :strips :negative-preconditions)
        (:predicates
                (conn ?x ?y)
                (key-shape ?k ?s)
                (lock-shape ?x ?s)
                (at ?r ?x)
                (at-robot ?x)
                (place ?p)
                (key ?k)
                (shape ?s)
                (locked ?x)
                (holding ?k)
                (open ?x)
                (arm-empty)

                (unlock ?curpos ?lockpos ?key ?shape)
                (move ?curpos ?nextpos)
                (pickup ?curpos ?key)
                (pickup-and-loose ?curpos ?newkey ?oldkey)
                (putdown ?curpos ?key)
        )

        (:action unlock
                :parameters (?curpos ?lockpos ?key ?shape)
                :precondition (and (place ?curpos) (place ?lockpos) (key ?key) (shape ?shape)
                        (conn ?curpos ?lockpos) (key-shape ?key ?shape)
                        (lock-shape ?lockpos ?shape) (at-robot ?curpos)
                        (locked ?lockpos) (holding ?key) (not(unlock ?curpos ?lockpos ?key ?shape)))
                :effect (oneof 
                        (and (open ?lockpos) (not (locked ?lockpos)))
                        (unlock ?curpos ?lockpos ?key ?shape))
        )

        (:action move
                :parameters (?curpos ?nextpos)
                :precondition (and (place ?curpos) (place ?nextpos)
                        (at-robot ?curpos) (conn ?curpos ?nextpos) (open ?nextpos) (not(move ?curpos ?nextpos)))
                :effect (oneof 
                        (and (at-robot ?nextpos) (not (at-robot ?curpos)))
                        (move ?curpos ?nextpos))
        )

        (:action pickup
                :parameters (?curpos ?key)
                :precondition (and (place ?curpos) (key ?key)
                        (at-robot ?curpos) (at ?key ?curpos) (arm-empty) (not(pickup ?curpos ?key)))
                :effect (oneof
                        (and (holding ?key) (not (at ?key ?curpos)) (not (arm-empty)))
                        (pickup ?curpos ?key))
        )

        (:action pickup-and-loose
                :parameters (?curpos ?newkey ?oldkey)
                :precondition (and (place ?curpos) (key ?newkey) (key ?oldkey)
                        (at-robot ?curpos) (holding ?oldkey) (at ?newkey ?curpos) (not(pickup-and-loose ?curpos ?newkey ?oldkey)))
                :effect (oneof
                        (and (holding ?newkey) (at ?oldkey ?curpos)
                        (not (holding ?oldkey)) (not (at ?newkey ?curpos)))
                        (pickup-and-loose ?curpos ?newkey ?oldkey)       
                        )
        )

        (:action putdown
                :parameters (?curpos ?key)
                :precondition (and (place ?curpos) (key ?key)
                        (at-robot ?curpos) (holding ?key) (not(putdown ?curpos ?key)))
                :effect (oneof
                        (and (arm-empty) (at ?key ?curpos) (not (holding ?key)))
                        (putdown ?curpos ?key) 
        )
        )
)