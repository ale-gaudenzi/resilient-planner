begin_version
3
end_version
begin_metric
0
end_metric
20
begin_variable
var0
-1
10
Atom at(hoist0, container-0-0)
Atom at(hoist0, container-0-1)
Atom at(hoist0, container-0-2)
Atom at(hoist0, depot0-1-1)
Atom at(hoist0, depot0-1-2)
Atom at(hoist0, depot0-1-3)
Atom at(hoist0, depot0-2-1)
Atom at(hoist0, depot0-2-2)
Atom at(hoist0, depot0-2-3)
Atom at(hoist0, loadarea)
end_variable
begin_variable
var1
-1
2
Atom available(hoist0)
NegatedAtom available(hoist0)
end_variable
begin_variable
var2
-1
2
Atom clear(container-0-0)
NegatedAtom clear(container-0-0)
end_variable
begin_variable
var3
-1
2
Atom clear(container-0-1)
NegatedAtom clear(container-0-1)
end_variable
begin_variable
var4
-1
2
Atom clear(container-0-2)
NegatedAtom clear(container-0-2)
end_variable
begin_variable
var5
-1
2
Atom clear(depot0-1-1)
NegatedAtom clear(depot0-1-1)
end_variable
begin_variable
var6
-1
2
Atom clear(depot0-1-2)
NegatedAtom clear(depot0-1-2)
end_variable
begin_variable
var7
-1
2
Atom clear(depot0-1-3)
NegatedAtom clear(depot0-1-3)
end_variable
begin_variable
var8
-1
2
Atom clear(depot0-2-1)
NegatedAtom clear(depot0-2-1)
end_variable
begin_variable
var9
-1
2
Atom clear(depot0-2-2)
NegatedAtom clear(depot0-2-2)
end_variable
begin_variable
var10
-1
2
Atom clear(depot0-2-3)
NegatedAtom clear(depot0-2-3)
end_variable
begin_variable
var11
-1
2
Atom in(crate0, container0)
NegatedAtom in(crate0, container0)
end_variable
begin_variable
var12
-1
2
Atom in(crate0, depot0)
NegatedAtom in(crate0, depot0)
end_variable
begin_variable
var13
-1
2
Atom in(crate1, container0)
NegatedAtom in(crate1, container0)
end_variable
begin_variable
var14
-1
2
Atom in(crate1, depot0)
NegatedAtom in(crate1, depot0)
end_variable
begin_variable
var15
-1
2
Atom in(crate2, container0)
NegatedAtom in(crate2, container0)
end_variable
begin_variable
var16
-1
2
Atom in(crate2, depot0)
NegatedAtom in(crate2, depot0)
end_variable
begin_variable
var17
-1
10
Atom lifting(hoist0, crate0)
Atom on(crate0, container-0-0)
Atom on(crate0, container-0-1)
Atom on(crate0, container-0-2)
Atom on(crate0, depot0-1-1)
Atom on(crate0, depot0-1-2)
Atom on(crate0, depot0-1-3)
Atom on(crate0, depot0-2-1)
Atom on(crate0, depot0-2-2)
Atom on(crate0, depot0-2-3)
end_variable
begin_variable
var18
-1
10
Atom lifting(hoist0, crate1)
Atom on(crate1, container-0-0)
Atom on(crate1, container-0-1)
Atom on(crate1, container-0-2)
Atom on(crate1, depot0-1-1)
Atom on(crate1, depot0-1-2)
Atom on(crate1, depot0-1-3)
Atom on(crate1, depot0-2-1)
Atom on(crate1, depot0-2-2)
Atom on(crate1, depot0-2-3)
end_variable
begin_variable
var19
-1
10
Atom lifting(hoist0, crate2)
Atom on(crate2, container-0-0)
Atom on(crate2, container-0-1)
Atom on(crate2, container-0-2)
Atom on(crate2, depot0-1-1)
Atom on(crate2, depot0-1-2)
Atom on(crate2, depot0-1-3)
Atom on(crate2, depot0-2-1)
Atom on(crate2, depot0-2-2)
Atom on(crate2, depot0-2-3)
end_variable
5
begin_mutex_group
10
0 0
0 1
0 2
0 3
0 4
0 5
0 6
0 7
0 8
0 9
end_mutex_group
begin_mutex_group
4
1 0
17 0
18 0
19 0
end_mutex_group
begin_mutex_group
10
17 0
17 1
17 2
17 3
17 4
17 5
17 6
17 7
17 8
17 9
end_mutex_group
begin_mutex_group
10
18 0
18 1
18 2
18 3
18 4
18 5
18 6
18 7
18 8
18 9
end_mutex_group
begin_mutex_group
10
19 0
19 1
19 2
19 3
19 4
19 5
19 6
19 7
19 8
19 9
end_mutex_group
begin_state
5
0
1
1
1
0
0
1
0
0
0
0
1
0
1
0
1
1
2
3
end_state
begin_goal
3
12 0
14 0
16 0
end_goal
130
begin_operator
drop hoist0 crate0 container-0-0 loadarea container0
1
0 9
4
0 1 -1 0
0 2 0 1
0 11 -1 0
0 17 0 1
0
end_operator
begin_operator
drop hoist0 crate0 container-0-1 loadarea container0
1
0 9
4
0 1 -1 0
0 3 0 1
0 11 -1 0
0 17 0 2
0
end_operator
begin_operator
drop hoist0 crate0 container-0-2 loadarea container0
1
0 9
4
0 1 -1 0
0 4 0 1
0 11 -1 0
0 17 0 3
0
end_operator
begin_operator
drop hoist0 crate0 depot0-1-1 depot0-1-2 depot0
1
0 4
4
0 1 -1 0
0 5 0 1
0 12 -1 0
0 17 0 4
0
end_operator
begin_operator
drop hoist0 crate0 depot0-1-1 depot0-2-1 depot0
1
0 6
4
0 1 -1 0
0 5 0 1
0 12 -1 0
0 17 0 4
0
end_operator
begin_operator
drop hoist0 crate0 depot0-1-2 depot0-1-1 depot0
1
0 3
4
0 1 -1 0
0 6 0 1
0 12 -1 0
0 17 0 5
0
end_operator
begin_operator
drop hoist0 crate0 depot0-1-2 depot0-1-3 depot0
1
0 5
4
0 1 -1 0
0 6 0 1
0 12 -1 0
0 17 0 5
0
end_operator
begin_operator
drop hoist0 crate0 depot0-1-2 depot0-2-2 depot0
1
0 7
4
0 1 -1 0
0 6 0 1
0 12 -1 0
0 17 0 5
0
end_operator
begin_operator
drop hoist0 crate0 depot0-1-3 depot0-1-2 depot0
1
0 4
4
0 1 -1 0
0 7 0 1
0 12 -1 0
0 17 0 6
0
end_operator
begin_operator
drop hoist0 crate0 depot0-1-3 depot0-2-3 depot0
1
0 8
4
0 1 -1 0
0 7 0 1
0 12 -1 0
0 17 0 6
0
end_operator
begin_operator
drop hoist0 crate0 depot0-2-1 depot0-1-1 depot0
1
0 3
4
0 1 -1 0
0 8 0 1
0 12 -1 0
0 17 0 7
0
end_operator
begin_operator
drop hoist0 crate0 depot0-2-1 depot0-2-2 depot0
1
0 7
4
0 1 -1 0
0 8 0 1
0 12 -1 0
0 17 0 7
0
end_operator
begin_operator
drop hoist0 crate0 depot0-2-1 loadarea depot0
1
0 9
4
0 1 -1 0
0 8 0 1
0 12 -1 0
0 17 0 7
0
end_operator
begin_operator
drop hoist0 crate0 depot0-2-2 depot0-1-2 depot0
1
0 4
4
0 1 -1 0
0 9 0 1
0 12 -1 0
0 17 0 8
0
end_operator
begin_operator
drop hoist0 crate0 depot0-2-2 depot0-2-1 depot0
1
0 6
4
0 1 -1 0
0 9 0 1
0 12 -1 0
0 17 0 8
0
end_operator
begin_operator
drop hoist0 crate0 depot0-2-2 depot0-2-3 depot0
1
0 8
4
0 1 -1 0
0 9 0 1
0 12 -1 0
0 17 0 8
0
end_operator
begin_operator
drop hoist0 crate0 depot0-2-3 depot0-1-3 depot0
1
0 5
4
0 1 -1 0
0 10 0 1
0 12 -1 0
0 17 0 9
0
end_operator
begin_operator
drop hoist0 crate0 depot0-2-3 depot0-2-2 depot0
1
0 7
4
0 1 -1 0
0 10 0 1
0 12 -1 0
0 17 0 9
0
end_operator
begin_operator
drop hoist0 crate1 container-0-0 loadarea container0
1
0 9
4
0 1 -1 0
0 2 0 1
0 13 -1 0
0 18 0 1
0
end_operator
begin_operator
drop hoist0 crate1 container-0-1 loadarea container0
1
0 9
4
0 1 -1 0
0 3 0 1
0 13 -1 0
0 18 0 2
0
end_operator
begin_operator
drop hoist0 crate1 container-0-2 loadarea container0
1
0 9
4
0 1 -1 0
0 4 0 1
0 13 -1 0
0 18 0 3
0
end_operator
begin_operator
drop hoist0 crate1 depot0-1-1 depot0-1-2 depot0
1
0 4
4
0 1 -1 0
0 5 0 1
0 14 -1 0
0 18 0 4
0
end_operator
begin_operator
drop hoist0 crate1 depot0-1-1 depot0-2-1 depot0
1
0 6
4
0 1 -1 0
0 5 0 1
0 14 -1 0
0 18 0 4
0
end_operator
begin_operator
drop hoist0 crate1 depot0-1-2 depot0-1-1 depot0
1
0 3
4
0 1 -1 0
0 6 0 1
0 14 -1 0
0 18 0 5
0
end_operator
begin_operator
drop hoist0 crate1 depot0-1-2 depot0-1-3 depot0
1
0 5
4
0 1 -1 0
0 6 0 1
0 14 -1 0
0 18 0 5
0
end_operator
begin_operator
drop hoist0 crate1 depot0-1-2 depot0-2-2 depot0
1
0 7
4
0 1 -1 0
0 6 0 1
0 14 -1 0
0 18 0 5
0
end_operator
begin_operator
drop hoist0 crate1 depot0-1-3 depot0-1-2 depot0
1
0 4
4
0 1 -1 0
0 7 0 1
0 14 -1 0
0 18 0 6
0
end_operator
begin_operator
drop hoist0 crate1 depot0-1-3 depot0-2-3 depot0
1
0 8
4
0 1 -1 0
0 7 0 1
0 14 -1 0
0 18 0 6
0
end_operator
begin_operator
drop hoist0 crate1 depot0-2-1 depot0-1-1 depot0
1
0 3
4
0 1 -1 0
0 8 0 1
0 14 -1 0
0 18 0 7
0
end_operator
begin_operator
drop hoist0 crate1 depot0-2-1 depot0-2-2 depot0
1
0 7
4
0 1 -1 0
0 8 0 1
0 14 -1 0
0 18 0 7
0
end_operator
begin_operator
drop hoist0 crate1 depot0-2-1 loadarea depot0
1
0 9
4
0 1 -1 0
0 8 0 1
0 14 -1 0
0 18 0 7
0
end_operator
begin_operator
drop hoist0 crate1 depot0-2-2 depot0-1-2 depot0
1
0 4
4
0 1 -1 0
0 9 0 1
0 14 -1 0
0 18 0 8
0
end_operator
begin_operator
drop hoist0 crate1 depot0-2-2 depot0-2-1 depot0
1
0 6
4
0 1 -1 0
0 9 0 1
0 14 -1 0
0 18 0 8
0
end_operator
begin_operator
drop hoist0 crate1 depot0-2-2 depot0-2-3 depot0
1
0 8
4
0 1 -1 0
0 9 0 1
0 14 -1 0
0 18 0 8
0
end_operator
begin_operator
drop hoist0 crate1 depot0-2-3 depot0-1-3 depot0
1
0 5
4
0 1 -1 0
0 10 0 1
0 14 -1 0
0 18 0 9
0
end_operator
begin_operator
drop hoist0 crate1 depot0-2-3 depot0-2-2 depot0
1
0 7
4
0 1 -1 0
0 10 0 1
0 14 -1 0
0 18 0 9
0
end_operator
begin_operator
drop hoist0 crate2 container-0-0 loadarea container0
1
0 9
4
0 1 -1 0
0 2 0 1
0 15 -1 0
0 19 0 1
0
end_operator
begin_operator
drop hoist0 crate2 container-0-1 loadarea container0
1
0 9
4
0 1 -1 0
0 3 0 1
0 15 -1 0
0 19 0 2
0
end_operator
begin_operator
drop hoist0 crate2 container-0-2 loadarea container0
1
0 9
4
0 1 -1 0
0 4 0 1
0 15 -1 0
0 19 0 3
0
end_operator
begin_operator
drop hoist0 crate2 depot0-1-1 depot0-1-2 depot0
1
0 4
4
0 1 -1 0
0 5 0 1
0 16 -1 0
0 19 0 4
0
end_operator
begin_operator
drop hoist0 crate2 depot0-1-1 depot0-2-1 depot0
1
0 6
4
0 1 -1 0
0 5 0 1
0 16 -1 0
0 19 0 4
0
end_operator
begin_operator
drop hoist0 crate2 depot0-1-2 depot0-1-1 depot0
1
0 3
4
0 1 -1 0
0 6 0 1
0 16 -1 0
0 19 0 5
0
end_operator
begin_operator
drop hoist0 crate2 depot0-1-2 depot0-1-3 depot0
1
0 5
4
0 1 -1 0
0 6 0 1
0 16 -1 0
0 19 0 5
0
end_operator
begin_operator
drop hoist0 crate2 depot0-1-2 depot0-2-2 depot0
1
0 7
4
0 1 -1 0
0 6 0 1
0 16 -1 0
0 19 0 5
0
end_operator
begin_operator
drop hoist0 crate2 depot0-1-3 depot0-1-2 depot0
1
0 4
4
0 1 -1 0
0 7 0 1
0 16 -1 0
0 19 0 6
0
end_operator
begin_operator
drop hoist0 crate2 depot0-1-3 depot0-2-3 depot0
1
0 8
4
0 1 -1 0
0 7 0 1
0 16 -1 0
0 19 0 6
0
end_operator
begin_operator
drop hoist0 crate2 depot0-2-1 depot0-1-1 depot0
1
0 3
4
0 1 -1 0
0 8 0 1
0 16 -1 0
0 19 0 7
0
end_operator
begin_operator
drop hoist0 crate2 depot0-2-1 depot0-2-2 depot0
1
0 7
4
0 1 -1 0
0 8 0 1
0 16 -1 0
0 19 0 7
0
end_operator
begin_operator
drop hoist0 crate2 depot0-2-1 loadarea depot0
1
0 9
4
0 1 -1 0
0 8 0 1
0 16 -1 0
0 19 0 7
0
end_operator
begin_operator
drop hoist0 crate2 depot0-2-2 depot0-1-2 depot0
1
0 4
4
0 1 -1 0
0 9 0 1
0 16 -1 0
0 19 0 8
0
end_operator
begin_operator
drop hoist0 crate2 depot0-2-2 depot0-2-1 depot0
1
0 6
4
0 1 -1 0
0 9 0 1
0 16 -1 0
0 19 0 8
0
end_operator
begin_operator
drop hoist0 crate2 depot0-2-2 depot0-2-3 depot0
1
0 8
4
0 1 -1 0
0 9 0 1
0 16 -1 0
0 19 0 8
0
end_operator
begin_operator
drop hoist0 crate2 depot0-2-3 depot0-1-3 depot0
1
0 5
4
0 1 -1 0
0 10 0 1
0 16 -1 0
0 19 0 9
0
end_operator
begin_operator
drop hoist0 crate2 depot0-2-3 depot0-2-2 depot0
1
0 7
4
0 1 -1 0
0 10 0 1
0 16 -1 0
0 19 0 9
0
end_operator
begin_operator
go-in hoist0 loadarea container-0-0
0
2
0 0 9 0
0 2 0 1
0
end_operator
begin_operator
go-in hoist0 loadarea container-0-1
0
2
0 0 9 1
0 3 0 1
0
end_operator
begin_operator
go-in hoist0 loadarea container-0-2
0
2
0 0 9 2
0 4 0 1
0
end_operator
begin_operator
go-in hoist0 loadarea depot0-2-1
0
2
0 0 9 6
0 8 0 1
0
end_operator
begin_operator
go-out hoist0 container-0-0 loadarea
0
2
0 0 0 9
0 2 -1 0
0
end_operator
begin_operator
go-out hoist0 container-0-1 loadarea
0
2
0 0 1 9
0 3 -1 0
0
end_operator
begin_operator
go-out hoist0 container-0-2 loadarea
0
2
0 0 2 9
0 4 -1 0
0
end_operator
begin_operator
go-out hoist0 depot0-2-1 loadarea
0
2
0 0 6 9
0 8 -1 0
0
end_operator
begin_operator
lift hoist0 crate0 container-0-0 loadarea container0
1
0 9
4
0 1 0 1
0 2 -1 0
0 11 -1 1
0 17 1 0
0
end_operator
begin_operator
lift hoist0 crate0 container-0-1 loadarea container0
1
0 9
4
0 1 0 1
0 3 -1 0
0 11 -1 1
0 17 2 0
0
end_operator
begin_operator
lift hoist0 crate0 container-0-2 loadarea container0
1
0 9
4
0 1 0 1
0 4 -1 0
0 11 -1 1
0 17 3 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-1-1 depot0-1-2 depot0
1
0 4
4
0 1 0 1
0 5 -1 0
0 12 -1 1
0 17 4 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-1-1 depot0-2-1 depot0
1
0 6
4
0 1 0 1
0 5 -1 0
0 12 -1 1
0 17 4 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-1-2 depot0-1-1 depot0
1
0 3
4
0 1 0 1
0 6 -1 0
0 12 -1 1
0 17 5 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-1-2 depot0-1-3 depot0
1
0 5
4
0 1 0 1
0 6 -1 0
0 12 -1 1
0 17 5 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-1-2 depot0-2-2 depot0
1
0 7
4
0 1 0 1
0 6 -1 0
0 12 -1 1
0 17 5 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-1-3 depot0-1-2 depot0
1
0 4
4
0 1 0 1
0 7 -1 0
0 12 -1 1
0 17 6 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-1-3 depot0-2-3 depot0
1
0 8
4
0 1 0 1
0 7 -1 0
0 12 -1 1
0 17 6 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-2-1 depot0-1-1 depot0
1
0 3
4
0 1 0 1
0 8 -1 0
0 12 -1 1
0 17 7 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-2-1 depot0-2-2 depot0
1
0 7
4
0 1 0 1
0 8 -1 0
0 12 -1 1
0 17 7 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-2-1 loadarea depot0
1
0 9
4
0 1 0 1
0 8 -1 0
0 12 -1 1
0 17 7 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-2-2 depot0-1-2 depot0
1
0 4
4
0 1 0 1
0 9 -1 0
0 12 -1 1
0 17 8 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-2-2 depot0-2-1 depot0
1
0 6
4
0 1 0 1
0 9 -1 0
0 12 -1 1
0 17 8 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-2-2 depot0-2-3 depot0
1
0 8
4
0 1 0 1
0 9 -1 0
0 12 -1 1
0 17 8 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-2-3 depot0-1-3 depot0
1
0 5
4
0 1 0 1
0 10 -1 0
0 12 -1 1
0 17 9 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-2-3 depot0-2-2 depot0
1
0 7
4
0 1 0 1
0 10 -1 0
0 12 -1 1
0 17 9 0
0
end_operator
begin_operator
lift hoist0 crate1 container-0-0 loadarea container0
1
0 9
4
0 1 0 1
0 2 -1 0
0 13 -1 1
0 18 1 0
0
end_operator
begin_operator
lift hoist0 crate1 container-0-1 loadarea container0
1
0 9
4
0 1 0 1
0 3 -1 0
0 13 -1 1
0 18 2 0
0
end_operator
begin_operator
lift hoist0 crate1 container-0-2 loadarea container0
1
0 9
4
0 1 0 1
0 4 -1 0
0 13 -1 1
0 18 3 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-1-1 depot0-1-2 depot0
1
0 4
4
0 1 0 1
0 5 -1 0
0 14 -1 1
0 18 4 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-1-1 depot0-2-1 depot0
1
0 6
4
0 1 0 1
0 5 -1 0
0 14 -1 1
0 18 4 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-1-2 depot0-1-1 depot0
1
0 3
4
0 1 0 1
0 6 -1 0
0 14 -1 1
0 18 5 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-1-2 depot0-1-3 depot0
1
0 5
4
0 1 0 1
0 6 -1 0
0 14 -1 1
0 18 5 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-1-2 depot0-2-2 depot0
1
0 7
4
0 1 0 1
0 6 -1 0
0 14 -1 1
0 18 5 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-1-3 depot0-1-2 depot0
1
0 4
4
0 1 0 1
0 7 -1 0
0 14 -1 1
0 18 6 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-1-3 depot0-2-3 depot0
1
0 8
4
0 1 0 1
0 7 -1 0
0 14 -1 1
0 18 6 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-2-1 depot0-1-1 depot0
1
0 3
4
0 1 0 1
0 8 -1 0
0 14 -1 1
0 18 7 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-2-1 depot0-2-2 depot0
1
0 7
4
0 1 0 1
0 8 -1 0
0 14 -1 1
0 18 7 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-2-1 loadarea depot0
1
0 9
4
0 1 0 1
0 8 -1 0
0 14 -1 1
0 18 7 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-2-2 depot0-1-2 depot0
1
0 4
4
0 1 0 1
0 9 -1 0
0 14 -1 1
0 18 8 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-2-2 depot0-2-1 depot0
1
0 6
4
0 1 0 1
0 9 -1 0
0 14 -1 1
0 18 8 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-2-2 depot0-2-3 depot0
1
0 8
4
0 1 0 1
0 9 -1 0
0 14 -1 1
0 18 8 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-2-3 depot0-1-3 depot0
1
0 5
4
0 1 0 1
0 10 -1 0
0 14 -1 1
0 18 9 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-2-3 depot0-2-2 depot0
1
0 7
4
0 1 0 1
0 10 -1 0
0 14 -1 1
0 18 9 0
0
end_operator
begin_operator
lift hoist0 crate2 container-0-0 loadarea container0
1
0 9
4
0 1 0 1
0 2 -1 0
0 15 -1 1
0 19 1 0
0
end_operator
begin_operator
lift hoist0 crate2 container-0-1 loadarea container0
1
0 9
4
0 1 0 1
0 3 -1 0
0 15 -1 1
0 19 2 0
0
end_operator
begin_operator
lift hoist0 crate2 container-0-2 loadarea container0
1
0 9
4
0 1 0 1
0 4 -1 0
0 15 -1 1
0 19 3 0
0
end_operator
begin_operator
lift hoist0 crate2 depot0-1-1 depot0-1-2 depot0
1
0 4
4
0 1 0 1
0 5 -1 0
0 16 -1 1
0 19 4 0
0
end_operator
begin_operator
lift hoist0 crate2 depot0-1-1 depot0-2-1 depot0
1
0 6
4
0 1 0 1
0 5 -1 0
0 16 -1 1
0 19 4 0
0
end_operator
begin_operator
lift hoist0 crate2 depot0-1-2 depot0-1-1 depot0
1
0 3
4
0 1 0 1
0 6 -1 0
0 16 -1 1
0 19 5 0
0
end_operator
begin_operator
lift hoist0 crate2 depot0-1-2 depot0-1-3 depot0
1
0 5
4
0 1 0 1
0 6 -1 0
0 16 -1 1
0 19 5 0
0
end_operator
begin_operator
lift hoist0 crate2 depot0-1-2 depot0-2-2 depot0
1
0 7
4
0 1 0 1
0 6 -1 0
0 16 -1 1
0 19 5 0
0
end_operator
begin_operator
lift hoist0 crate2 depot0-1-3 depot0-1-2 depot0
1
0 4
4
0 1 0 1
0 7 -1 0
0 16 -1 1
0 19 6 0
0
end_operator
begin_operator
lift hoist0 crate2 depot0-1-3 depot0-2-3 depot0
1
0 8
4
0 1 0 1
0 7 -1 0
0 16 -1 1
0 19 6 0
0
end_operator
begin_operator
lift hoist0 crate2 depot0-2-1 depot0-1-1 depot0
1
0 3
4
0 1 0 1
0 8 -1 0
0 16 -1 1
0 19 7 0
0
end_operator
begin_operator
lift hoist0 crate2 depot0-2-1 depot0-2-2 depot0
1
0 7
4
0 1 0 1
0 8 -1 0
0 16 -1 1
0 19 7 0
0
end_operator
begin_operator
lift hoist0 crate2 depot0-2-1 loadarea depot0
1
0 9
4
0 1 0 1
0 8 -1 0
0 16 -1 1
0 19 7 0
0
end_operator
begin_operator
lift hoist0 crate2 depot0-2-2 depot0-1-2 depot0
1
0 4
4
0 1 0 1
0 9 -1 0
0 16 -1 1
0 19 8 0
0
end_operator
begin_operator
lift hoist0 crate2 depot0-2-2 depot0-2-1 depot0
1
0 6
4
0 1 0 1
0 9 -1 0
0 16 -1 1
0 19 8 0
0
end_operator
begin_operator
lift hoist0 crate2 depot0-2-2 depot0-2-3 depot0
1
0 8
4
0 1 0 1
0 9 -1 0
0 16 -1 1
0 19 8 0
0
end_operator
begin_operator
lift hoist0 crate2 depot0-2-3 depot0-1-3 depot0
1
0 5
4
0 1 0 1
0 10 -1 0
0 16 -1 1
0 19 9 0
0
end_operator
begin_operator
lift hoist0 crate2 depot0-2-3 depot0-2-2 depot0
1
0 7
4
0 1 0 1
0 10 -1 0
0 16 -1 1
0 19 9 0
0
end_operator
begin_operator
move hoist0 depot0-1-1 depot0-1-2
0
3
0 0 3 4
0 5 -1 0
0 6 0 1
0
end_operator
begin_operator
move hoist0 depot0-1-1 depot0-2-1
0
3
0 0 3 6
0 5 -1 0
0 8 0 1
0
end_operator
begin_operator
move hoist0 depot0-1-2 depot0-1-1
0
3
0 0 4 3
0 5 0 1
0 6 -1 0
0
end_operator
begin_operator
move hoist0 depot0-1-2 depot0-1-3
0
3
0 0 4 5
0 6 -1 0
0 7 0 1
0
end_operator
begin_operator
move hoist0 depot0-1-2 depot0-2-2
0
3
0 0 4 7
0 6 -1 0
0 9 0 1
0
end_operator
begin_operator
move hoist0 depot0-1-3 depot0-1-2
0
3
0 0 5 4
0 6 0 1
0 7 -1 0
0
end_operator
begin_operator
move hoist0 depot0-1-3 depot0-2-3
0
3
0 0 5 8
0 7 -1 0
0 10 0 1
0
end_operator
begin_operator
move hoist0 depot0-2-1 depot0-1-1
0
3
0 0 6 3
0 5 0 1
0 8 -1 0
0
end_operator
begin_operator
move hoist0 depot0-2-1 depot0-2-2
0
3
0 0 6 7
0 8 -1 0
0 9 0 1
0
end_operator
begin_operator
move hoist0 depot0-2-2 depot0-1-2
0
3
0 0 7 4
0 6 0 1
0 9 -1 0
0
end_operator
begin_operator
move hoist0 depot0-2-2 depot0-2-1
0
3
0 0 7 6
0 8 0 1
0 9 -1 0
0
end_operator
begin_operator
move hoist0 depot0-2-2 depot0-2-3
0
3
0 0 7 8
0 9 -1 0
0 10 0 1
0
end_operator
begin_operator
move hoist0 depot0-2-3 depot0-1-3
0
3
0 0 8 5
0 7 0 1
0 10 -1 0
0
end_operator
begin_operator
move hoist0 depot0-2-3 depot0-2-2
0
3
0 0 8 7
0 9 0 1
0 10 -1 0
0
end_operator
0
