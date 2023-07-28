(define (problem BLOCKS-4-0) (:domain blocks)
(:objects
	a - block
	c - block
	b - block
	e - block
	d - block
	g - block
	f - block
	i - block
	h - block
	a1 - agent
	a2 - agent
	a3 - agent
)
(:init
	(handempty a1)
	(handempty a2)
	(handempty a3)
	(clear c)
	(clear f)
	(clear h)
	(clear a)
	(ontable c)
	(ontable b)
	(ontable d)
	(ontable e)
	(on f g)
	(on g e)
	(on a i)
	(on i d)
	(on h b)
)
(:goal
	(and
		(on c a)
	)
)
)
