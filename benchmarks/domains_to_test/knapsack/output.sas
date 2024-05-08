begin_version
3
end_version
begin_metric
0
end_metric
6
begin_variable
var0
-1
2
Atom holding(a0)
NegatedAtom holding(a0)
end_variable
begin_variable
var1
-1
2
Atom holding(a1)
NegatedAtom holding(a1)
end_variable
begin_variable
var2
-1
2
Atom in(pencil, k0)
NegatedAtom in(pencil, k0)
end_variable
begin_variable
var3
-1
2
Atom is_close(k0)
NegatedAtom is_close(k0)
end_variable
begin_variable
var4
-1
2
Atom is_hold(pencil, a0)
NegatedAtom is_hold(pencil, a0)
end_variable
begin_variable
var5
-1
2
Atom is_hold(pencil, a1)
NegatedAtom is_hold(pencil, a1)
end_variable
0
begin_state
1
1
1
0
1
1
end_state
begin_goal
2
2 0
3 0
end_goal
7
begin_operator
close k0 a0
1
0 1
1
0 3 1 0
0
end_operator
begin_operator
hold a0 pencil
0
2
0 0 1 0
0 4 -1 0
0
end_operator
begin_operator
hold a1 pencil
0
2
0 1 1 0
0 5 -1 0
0
end_operator
begin_operator
open k0 a0
1
0 1
1
0 3 0 1
0
end_operator
begin_operator
open k0 a1
1
1 1
1
0 3 0 1
0
end_operator
begin_operator
put_in_knapsack k0 a0 pencil
2
3 1
4 0
2
0 0 -1 1
0 2 -1 0
0
end_operator
begin_operator
put_in_knapsack k0 a1 pencil
2
3 1
5 0
2
0 1 -1 1
0 2 -1 0
0
end_operator
0
