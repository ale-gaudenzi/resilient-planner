(define (domain satellite)
    (:requirements :equality :strips)
    (:predicates
        (on_board ?i ?s)
        (supports ?i ?m)
        (pointing ?s ?d)
        (power_avail ?s)
        (power_on ?i)
        (calibrated ?i)
        (have_image ?d ?m)
        (calibration_target ?i ?d)(satellite ?x)
        (direction ?x)
        (instrument ?x)
        (mode ?x)
        (turn_to ?s ?d_new ?d_prev)
        (switch_on ?i ?s)
        (switch_off ?i ?s)
        (calibrate ?s ?i ?d)
        (take_image ?s ?d ?i ?m)
    )

    (:action turn_to
        :parameters ( ?s ?d_new ?d_prev)
        :precondition (and (satellite ?s) (direction ?d_new) (direction ?d_prev) (pointing ?s ?d_prev)
            (not(turn_to ?s ?d_new ?d_prev)))
        :effect (oneof
            (and (pointing ?s ?d_new) (not (pointing ?s ?d_prev)))
            (turn_to ?s ?d_new ?d_prev))
    )

    (:action switch_on
        :parameters ( ?i ?s)
        :precondition (and (instrument ?i) (satellite ?s) (on_board ?i ?s) (power_avail ?s)
            (not(switch_on ?i ?s)))
        :effect (oneof
            (and (power_on ?i) (not (calibrated ?i)) (not (power_avail ?s)))
            (switch_on ?i ?s)

        )
    )

    (:action switch_off
        :parameters ( ?i ?s)
        :precondition (and (instrument ?i) (satellite ?s) (on_board ?i ?s) (power_on ?i)
            (not(switch_off ?i ?s)))
        :effect (oneof
            (and (power_avail ?s) (not (power_on ?i)))(switch_off ?i ?s))
    )

    (:action calibrate
        :parameters ( ?s ?i ?d)
        :precondition (and (satellite ?s) (instrument ?i) (direction ?d) (on_board ?i ?s) (calibration_target ?i ?d) (pointing ?s ?d) (power_on ?i)
            (not(calibrate ?s ?i ?d)))
        :effect (oneof(calibrated ?i)(calibrate ?s ?i ?d))
    )

    (:action take_image
        :parameters ( ?s ?d ?i ?m)
        :precondition (and (satellite ?s) (direction ?d) (instrument ?i) (mode ?m) (calibrated ?i) (on_board ?i ?s) (supports ?i ?m) (power_on ?i) (pointing ?s ?d) (power_on ?i)
            (not(take_image ?s ?d ?i ?m)))
        :effect (oneof(have_image ?d ?m)(take_image ?s ?d ?i ?m))
    )

)