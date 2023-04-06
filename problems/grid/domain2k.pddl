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

                (faults_0)
                (faults_1)
                (faults_2)
        )

        (:action unlock-0
                :parameters (?curpos ?lockpos ?key ?shape)
                :precondition (and (place ?curpos) (place ?lockpos) (key ?key) (shape ?shape)
                        (conn ?curpos ?lockpos) (key-shape ?key ?shape)
                        (lock-shape ?lockpos ?shape) (at-robot ?curpos)
                        (locked ?lockpos) (holding ?key) (not(unlock ?curpos ?lockpos ?key ?shape)) (faults_0)
                )
                :effect (oneof
                        (and (open ?lockpos) (not (locked ?lockpos)))
                        (and(faults_1)
                                (not(faults_0))(unlock ?curpos ?lockpos ?key ?shape))
                )
        )
        (:action move-0
                :parameters (?curpos ?nextpos)
                :precondition (and (place ?curpos) (place ?nextpos)
                        (at-robot ?curpos) (conn ?curpos ?nextpos) (open ?nextpos) (not(move ?curpos ?nextpos)) (faults_0)
                )
                :effect (oneof
                        (and (at-robot ?nextpos) (not (at-robot ?curpos)))
                        (and(faults_1)
                                (not(faults_0))(move ?curpos ?nextpos)))
        )

        (:action pickup-0
                :parameters (?curpos ?key)
                :precondition (and (place ?curpos) (key ?key)
                        (at-robot ?curpos) (at ?key ?curpos) (arm-empty) (not(pickup ?curpos ?key)) (faults_0))
                :effect (oneof
                        (and (holding ?key) (not (at ?key ?curpos)) (not (arm-empty)))
                        (and(faults_1)
                                (not(faults_0))(pickup ?curpos ?key))
                )
        )

        (:action pickup-and-loose-0
                :parameters (?curpos ?newkey ?oldkey)
                :precondition (and (place ?curpos) (key ?newkey) (key ?oldkey)
                        (at-robot ?curpos) (holding ?oldkey) (at ?newkey ?curpos) (not(pickup-and-loose ?curpos ?newkey ?oldkey)) (faults_0))
                :effect (oneof
                        (and (holding ?newkey) (at ?oldkey ?curpos)
                                (not (holding ?oldkey)) (not (at ?newkey ?curpos)))
                        (and(faults_1)
                                (not(faults_0))(pickup-and-loose ?curpos ?newkey ?oldkey))
                )
        )

        (:action putdown-0
                :parameters (?curpos ?key)
                :precondition (and (place ?curpos) (key ?key)
                        (at-robot ?curpos) (holding ?key) (not(putdown ?curpos ?key)) (faults_0))
                :effect (oneof
                        (and (arm-empty) (at ?key ?curpos) (not (holding ?key)))
                        (and(faults_1)
                                (not(faults_0))(putdown ?curpos ?key))
                )
        )
        ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        
        
        (:action unlock-1
                :parameters (?curpos ?lockpos ?key ?shape)
                :precondition (and (place ?curpos) (place ?lockpos) (key ?key) (shape ?shape)
                        (conn ?curpos ?lockpos) (key-shape ?key ?shape)
                        (lock-shape ?lockpos ?shape) (at-robot ?curpos)
                        (locked ?lockpos) (holding ?key) (not(unlock ?curpos ?lockpos ?key ?shape)) (faults_1)
                )
                :effect (oneof
                        (and (open ?lockpos) (not (locked ?lockpos)))
                        (and(faults_2)
                                (not(faults_1))(unlock ?curpos ?lockpos ?key ?shape))
                )
        )
        (:action move-1
                :parameters (?curpos ?nextpos)
                :precondition (and (place ?curpos) (place ?nextpos)
                        (at-robot ?curpos) (conn ?curpos ?nextpos) (open ?nextpos) (not(move ?curpos ?nextpos)) (faults_1)
                )
                :effect (oneof
                        (and (at-robot ?nextpos) (not (at-robot ?curpos)))
                        (and(faults_2)
                                (not(faults_1))(move ?curpos ?nextpos)))
        )

        (:action pickup-1
                :parameters (?curpos ?key)
                :precondition (and (place ?curpos) (key ?key)
                        (at-robot ?curpos) (at ?key ?curpos) (arm-empty) (not(pickup ?curpos ?key)) (faults_1))
                :effect (oneof
                        (and (holding ?key) (not (at ?key ?curpos)) (not (arm-empty)))
                        (and(faults_2)
                                (not(faults_1))(pickup ?curpos ?key))
                )
        )

        (:action pickup-and-loose-1
                :parameters (?curpos ?newkey ?oldkey)
                :precondition (and (place ?curpos) (key ?newkey) (key ?oldkey)
                        (at-robot ?curpos) (holding ?oldkey) (at ?newkey ?curpos) (not(pickup-and-loose ?curpos ?newkey ?oldkey)) (faults_1))
                :effect (oneof
                        (and (holding ?newkey) (at ?oldkey ?curpos)
                                (not (holding ?oldkey)) (not (at ?newkey ?curpos)))
                        (and(faults_2)
                                (not(faults_1))(pickup-and-loose ?curpos ?newkey ?oldkey))
                )
        )

        (:action putdown-1
                :parameters (?curpos ?key)
                :precondition (and (place ?curpos) (key ?key)
                        (at-robot ?curpos) (holding ?key) (not(putdown ?curpos ?key)) (faults_1))
                :effect (oneof
                        (and (arm-empty) (at ?key ?curpos) (not (holding ?key)))
                        (and(faults_2)
                                (not(faults_1))(putdown ?curpos ?key))
                )
        )

        ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


        (:action unlock-2
                :parameters (?curpos ?lockpos ?key ?shape)
                :precondition (and (place ?curpos) (place ?lockpos) (key ?key) (shape ?shape)
                        (conn ?curpos ?lockpos) (key-shape ?key ?shape)
                        (lock-shape ?lockpos ?shape) (at-robot ?curpos)
                        (locked ?lockpos) (holding ?key) (not(unlock ?curpos ?lockpos ?key ?shape)) (faults_2))
                :effect (and (open ?lockpos) (not (locked ?lockpos)))
        )

        (:action move-2
                :parameters (?curpos ?nextpos)
                :precondition (and (place ?curpos) (place ?nextpos)
                        (at-robot ?curpos) (conn ?curpos ?nextpos) (open ?nextpos) (not(move ?curpos ?nextpos))(faults_2))
                :effect (and (at-robot ?nextpos) (not (at-robot ?curpos)))
        )

        (:action pickup-2
                :parameters (?curpos ?key)
                :precondition (and (place ?curpos) (key ?key)
                        (at-robot ?curpos) (at ?key ?curpos) (arm-empty) (not(pickup ?curpos ?key))(faults_2))
                :effect (and (holding ?key) (not (at ?key ?curpos)) (not (arm-empty)))
        )

        (:action pickup-and-loose-2
                :parameters (?curpos ?newkey ?oldkey)
                :precondition (and (place ?curpos) (key ?newkey) (key ?oldkey)
                        (at-robot ?curpos) (holding ?oldkey) (at ?newkey ?curpos) (not(pickup-and-loose ?curpos ?newkey ?oldkey))(faults_2))
                :effect (and (holding ?newkey) (at ?oldkey ?curpos)
                        (not (holding ?oldkey)) (not (at ?newkey ?curpos)))
        )

        (:action putdown-2
                :parameters (?curpos ?key)
                :precondition (and (place ?curpos) (key ?key)
                        (at-robot ?curpos) (holding ?key) (not(putdown ?curpos ?key))(faults_2))
                :effect (and (arm-empty) (at ?key ?curpos) (not (holding ?key)))
        )

)