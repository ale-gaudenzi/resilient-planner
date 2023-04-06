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
        (faults_0)
        (faults_1)
    )

    (:action turn_to-0
        :parameters ( ?s ?d_new ?d_prev)
        :precondition (and (faults_0)(satellite ?s) (direction ?d_new) (direction ?d_prev) (pointing ?s ?d_prev)
            (not(turn_to ?s ?d_new ?d_prev)))
        :effect (oneof
            (and (pointing ?s ?d_new) (not (pointing ?s ?d_prev)))
            (and(faults_1)(not(faults_0))(turn_to ?s ?d_new ?d_prev))    
        )
    )

    (:action switch_on-0
        :parameters ( ?i ?s)
        :precondition (and (faults_0)(instrument ?i) (satellite ?s) (on_board ?i ?s) (power_avail ?s)
            (not(switch_on ?i ?s)))
        :effect (oneof
            (and (power_on ?i) (not (calibrated ?i)) (not (power_avail ?s)))
            (and(faults_1)(not(faults_0))(switch_on ?i ?s))

        )
    )

    (:action switch_off-0
        :parameters ( ?i ?s)
        :precondition (and (faults_0)(instrument ?i) (satellite ?s) (on_board ?i ?s) (power_on ?i)
            (not(switch_off ?i ?s)))
        :effect (oneof
            (and (power_avail ?s) (not (power_on ?i)))
            (and(faults_1)(not(faults_0))(switch_off ?i ?s)))
    )

    (:action calibrate-0
        :parameters ( ?s ?i ?d)
        :precondition (and (faults_0)(satellite ?s) (instrument ?i) (direction ?d) (on_board ?i ?s) (calibration_target ?i ?d) (pointing ?s ?d) (power_on ?i)
            (not(calibrate ?s ?i ?d)))
        :effect (oneof(calibrated ?i)
        (and(faults_1)(not(faults_0))(calibrate ?s ?i ?d)))
    )

    (:action take_image-0
        :parameters ( ?s ?d ?i ?m)
        :precondition (and (faults_0)(satellite ?s) (direction ?d) (instrument ?i) (mode ?m) (calibrated ?i) (on_board ?i ?s) (supports ?i ?m) (power_on ?i) (pointing ?s ?d) (power_on ?i)
            (not(take_image ?s ?d ?i ?m)))
        :effect (oneof(have_image ?d ?m)(and(faults_1)(not(faults_0))(take_image ?s ?d ?i ?m)))
    )

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    (:action turn_to-1
        :parameters ( ?s ?d_new ?d_prev)
        :precondition (and (faults_1)(satellite ?s) (direction ?d_new) (direction ?d_prev) (pointing ?s ?d_prev)
            (not(turn_to ?s ?d_new ?d_prev)))
        :effect 
            (and (pointing ?s ?d_new) (not (pointing ?s ?d_prev)))
    )

    (:action switch_on-1
        :parameters ( ?i ?s)
        :precondition (and (faults_1)(instrument ?i) (satellite ?s) (on_board ?i ?s) (power_avail ?s)
            (not(switch_on ?i ?s)))
        :effect 
            (and (power_on ?i) (not (calibrated ?i)) (not (power_avail ?s)))
    )

    (:action switch_off-1
        :parameters ( ?i ?s)
        :precondition (and (faults_1)(instrument ?i) (satellite ?s) (on_board ?i ?s) (power_on ?i)
            (not(switch_off ?i ?s)))
        :effect 
            (and (power_avail ?s) (not (power_on ?i)))
    )

    (:action calibrate-1
        :parameters ( ?s ?i ?d)
        :precondition (and (faults_1)(satellite ?s) (instrument ?i) (direction ?d) (on_board ?i ?s) (calibration_target ?i ?d) (pointing ?s ?d) (power_on ?i)
            (not(calibrate ?s ?i ?d)))
        :effect (calibrated ?i)
    )

    (:action take_image-1
        :parameters ( ?s ?d ?i ?m)
        :precondition (and (faults_1)(satellite ?s) (direction ?d) (instrument ?i) (mode ?m) (calibrated ?i) (on_board ?i ?s) (supports ?i ?m) (power_on ?i) (pointing ?s ?d) (power_on ?i)
            (not(take_image ?s ?d ?i ?m)))
        :effect (have_image ?d ?m)
    )

)