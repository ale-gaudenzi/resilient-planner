begin_version
3
end_version
begin_metric
0
end_metric
18
begin_variable
var0
-1
7
Atom at(hoist0, container-0-0)
Atom at(hoist0, container-0-1)
Atom at(hoist0, depot0-1-1)
Atom at(hoist0, depot0-1-2)
Atom at(hoist0, depot0-2-1)
Atom at(hoist0, depot0-2-2)
Atom at(hoist0, loadarea)
end_variable
begin_variable
var1
-1
7
Atom at(hoist1, container-0-0)
Atom at(hoist1, container-0-1)
Atom at(hoist1, depot0-1-1)
Atom at(hoist1, depot0-1-2)
Atom at(hoist1, depot0-2-1)
Atom at(hoist1, depot0-2-2)
Atom at(hoist1, loadarea)
end_variable
begin_variable
var2
-1
7
Atom at(hoist2, container-0-0)
Atom at(hoist2, container-0-1)
Atom at(hoist2, depot0-1-1)
Atom at(hoist2, depot0-1-2)
Atom at(hoist2, depot0-2-1)
Atom at(hoist2, depot0-2-2)
Atom at(hoist2, loadarea)
end_variable
begin_variable
var3
-1
2
Atom available(hoist0)
NegatedAtom available(hoist0)
end_variable
begin_variable
var4
-1
2
Atom available(hoist1)
NegatedAtom available(hoist1)
end_variable
begin_variable
var5
-1
2
Atom available(hoist2)
NegatedAtom available(hoist2)
end_variable
begin_variable
var6
-1
2
Atom clear(container-0-0)
NegatedAtom clear(container-0-0)
end_variable
begin_variable
var7
-1
2
Atom clear(container-0-1)
NegatedAtom clear(container-0-1)
end_variable
begin_variable
var8
-1
2
Atom clear(depot0-1-1)
NegatedAtom clear(depot0-1-1)
end_variable
begin_variable
var9
-1
2
Atom clear(depot0-1-2)
NegatedAtom clear(depot0-1-2)
end_variable
begin_variable
var10
-1
2
Atom clear(depot0-2-1)
NegatedAtom clear(depot0-2-1)
end_variable
begin_variable
var11
-1
2
Atom clear(depot0-2-2)
NegatedAtom clear(depot0-2-2)
end_variable
begin_variable
var12
-1
2
Atom in(crate0, container0)
NegatedAtom in(crate0, container0)
end_variable
begin_variable
var13
-1
2
Atom in(crate0, depot0)
NegatedAtom in(crate0, depot0)
end_variable
begin_variable
var14
-1
2
Atom in(crate1, container0)
NegatedAtom in(crate1, container0)
end_variable
begin_variable
var15
-1
2
Atom in(crate1, depot0)
NegatedAtom in(crate1, depot0)
end_variable
begin_variable
var16
-1
9
Atom lifting(hoist0, crate0)
Atom lifting(hoist1, crate0)
Atom lifting(hoist2, crate0)
Atom on(crate0, container-0-0)
Atom on(crate0, container-0-1)
Atom on(crate0, depot0-1-1)
Atom on(crate0, depot0-1-2)
Atom on(crate0, depot0-2-1)
Atom on(crate0, depot0-2-2)
end_variable
begin_variable
var17
-1
9
Atom lifting(hoist0, crate1)
Atom lifting(hoist1, crate1)
Atom lifting(hoist2, crate1)
Atom on(crate1, container-0-0)
Atom on(crate1, container-0-1)
Atom on(crate1, depot0-1-1)
Atom on(crate1, depot0-1-2)
Atom on(crate1, depot0-2-1)
Atom on(crate1, depot0-2-2)
end_variable
8
begin_mutex_group
7
0 0
0 1
0 2
0 3
0 4
0 5
0 6
end_mutex_group
begin_mutex_group
7
1 0
1 1
1 2
1 3
1 4
1 5
1 6
end_mutex_group
begin_mutex_group
7
2 0
2 1
2 2
2 3
2 4
2 5
2 6
end_mutex_group
begin_mutex_group
3
3 0
16 0
17 0
end_mutex_group
begin_mutex_group
3
4 0
16 1
17 1
end_mutex_group
begin_mutex_group
3
5 0
16 2
17 2
end_mutex_group
begin_mutex_group
9
16 0
16 1
16 2
16 3
16 4
16 5
16 6
16 7
16 8
end_mutex_group
begin_mutex_group
9
17 0
17 1
17 2
17 3
17 4
17 5
17 6
17 7
17 8
end_mutex_group
begin_state
2
3
5
0
0
0
1
1
1
1
0
1
0
1
0
1
3
4
end_state
begin_goal
2
13 0
15 0
end_goal
174
begin_operator
drop hoist0 crate0 container-0-0 loadarea container0
1
0 6
4
0 3 -1 0
0 6 0 1
0 12 -1 0
0 16 0 3
0
end_operator
begin_operator
drop hoist0 crate0 container-0-1 loadarea container0
1
0 6
4
0 3 -1 0
0 7 0 1
0 12 -1 0
0 16 0 4
0
end_operator
begin_operator
drop hoist0 crate0 depot0-1-1 depot0-1-2 depot0
1
0 3
4
0 3 -1 0
0 8 0 1
0 13 -1 0
0 16 0 5
0
end_operator
begin_operator
drop hoist0 crate0 depot0-1-1 depot0-2-1 depot0
1
0 4
4
0 3 -1 0
0 8 0 1
0 13 -1 0
0 16 0 5
0
end_operator
begin_operator
drop hoist0 crate0 depot0-1-2 depot0-1-1 depot0
1
0 2
4
0 3 -1 0
0 9 0 1
0 13 -1 0
0 16 0 6
0
end_operator
begin_operator
drop hoist0 crate0 depot0-1-2 depot0-2-2 depot0
1
0 5
4
0 3 -1 0
0 9 0 1
0 13 -1 0
0 16 0 6
0
end_operator
begin_operator
drop hoist0 crate0 depot0-2-1 depot0-1-1 depot0
1
0 2
4
0 3 -1 0
0 10 0 1
0 13 -1 0
0 16 0 7
0
end_operator
begin_operator
drop hoist0 crate0 depot0-2-1 depot0-2-2 depot0
1
0 5
4
0 3 -1 0
0 10 0 1
0 13 -1 0
0 16 0 7
0
end_operator
begin_operator
drop hoist0 crate0 depot0-2-1 loadarea depot0
1
0 6
4
0 3 -1 0
0 10 0 1
0 13 -1 0
0 16 0 7
0
end_operator
begin_operator
drop hoist0 crate0 depot0-2-2 depot0-1-2 depot0
1
0 3
4
0 3 -1 0
0 11 0 1
0 13 -1 0
0 16 0 8
0
end_operator
begin_operator
drop hoist0 crate0 depot0-2-2 depot0-2-1 depot0
1
0 4
4
0 3 -1 0
0 11 0 1
0 13 -1 0
0 16 0 8
0
end_operator
begin_operator
drop hoist0 crate1 container-0-0 loadarea container0
1
0 6
4
0 3 -1 0
0 6 0 1
0 14 -1 0
0 17 0 3
0
end_operator
begin_operator
drop hoist0 crate1 container-0-1 loadarea container0
1
0 6
4
0 3 -1 0
0 7 0 1
0 14 -1 0
0 17 0 4
0
end_operator
begin_operator
drop hoist0 crate1 depot0-1-1 depot0-1-2 depot0
1
0 3
4
0 3 -1 0
0 8 0 1
0 15 -1 0
0 17 0 5
0
end_operator
begin_operator
drop hoist0 crate1 depot0-1-1 depot0-2-1 depot0
1
0 4
4
0 3 -1 0
0 8 0 1
0 15 -1 0
0 17 0 5
0
end_operator
begin_operator
drop hoist0 crate1 depot0-1-2 depot0-1-1 depot0
1
0 2
4
0 3 -1 0
0 9 0 1
0 15 -1 0
0 17 0 6
0
end_operator
begin_operator
drop hoist0 crate1 depot0-1-2 depot0-2-2 depot0
1
0 5
4
0 3 -1 0
0 9 0 1
0 15 -1 0
0 17 0 6
0
end_operator
begin_operator
drop hoist0 crate1 depot0-2-1 depot0-1-1 depot0
1
0 2
4
0 3 -1 0
0 10 0 1
0 15 -1 0
0 17 0 7
0
end_operator
begin_operator
drop hoist0 crate1 depot0-2-1 depot0-2-2 depot0
1
0 5
4
0 3 -1 0
0 10 0 1
0 15 -1 0
0 17 0 7
0
end_operator
begin_operator
drop hoist0 crate1 depot0-2-1 loadarea depot0
1
0 6
4
0 3 -1 0
0 10 0 1
0 15 -1 0
0 17 0 7
0
end_operator
begin_operator
drop hoist0 crate1 depot0-2-2 depot0-1-2 depot0
1
0 3
4
0 3 -1 0
0 11 0 1
0 15 -1 0
0 17 0 8
0
end_operator
begin_operator
drop hoist0 crate1 depot0-2-2 depot0-2-1 depot0
1
0 4
4
0 3 -1 0
0 11 0 1
0 15 -1 0
0 17 0 8
0
end_operator
begin_operator
drop hoist1 crate0 container-0-0 loadarea container0
1
1 6
4
0 4 -1 0
0 6 0 1
0 12 -1 0
0 16 1 3
0
end_operator
begin_operator
drop hoist1 crate0 container-0-1 loadarea container0
1
1 6
4
0 4 -1 0
0 7 0 1
0 12 -1 0
0 16 1 4
0
end_operator
begin_operator
drop hoist1 crate0 depot0-1-1 depot0-1-2 depot0
1
1 3
4
0 4 -1 0
0 8 0 1
0 13 -1 0
0 16 1 5
0
end_operator
begin_operator
drop hoist1 crate0 depot0-1-1 depot0-2-1 depot0
1
1 4
4
0 4 -1 0
0 8 0 1
0 13 -1 0
0 16 1 5
0
end_operator
begin_operator
drop hoist1 crate0 depot0-1-2 depot0-1-1 depot0
1
1 2
4
0 4 -1 0
0 9 0 1
0 13 -1 0
0 16 1 6
0
end_operator
begin_operator
drop hoist1 crate0 depot0-1-2 depot0-2-2 depot0
1
1 5
4
0 4 -1 0
0 9 0 1
0 13 -1 0
0 16 1 6
0
end_operator
begin_operator
drop hoist1 crate0 depot0-2-1 depot0-1-1 depot0
1
1 2
4
0 4 -1 0
0 10 0 1
0 13 -1 0
0 16 1 7
0
end_operator
begin_operator
drop hoist1 crate0 depot0-2-1 depot0-2-2 depot0
1
1 5
4
0 4 -1 0
0 10 0 1
0 13 -1 0
0 16 1 7
0
end_operator
begin_operator
drop hoist1 crate0 depot0-2-1 loadarea depot0
1
1 6
4
0 4 -1 0
0 10 0 1
0 13 -1 0
0 16 1 7
0
end_operator
begin_operator
drop hoist1 crate0 depot0-2-2 depot0-1-2 depot0
1
1 3
4
0 4 -1 0
0 11 0 1
0 13 -1 0
0 16 1 8
0
end_operator
begin_operator
drop hoist1 crate0 depot0-2-2 depot0-2-1 depot0
1
1 4
4
0 4 -1 0
0 11 0 1
0 13 -1 0
0 16 1 8
0
end_operator
begin_operator
drop hoist1 crate1 container-0-0 loadarea container0
1
1 6
4
0 4 -1 0
0 6 0 1
0 14 -1 0
0 17 1 3
0
end_operator
begin_operator
drop hoist1 crate1 container-0-1 loadarea container0
1
1 6
4
0 4 -1 0
0 7 0 1
0 14 -1 0
0 17 1 4
0
end_operator
begin_operator
drop hoist1 crate1 depot0-1-1 depot0-1-2 depot0
1
1 3
4
0 4 -1 0
0 8 0 1
0 15 -1 0
0 17 1 5
0
end_operator
begin_operator
drop hoist1 crate1 depot0-1-1 depot0-2-1 depot0
1
1 4
4
0 4 -1 0
0 8 0 1
0 15 -1 0
0 17 1 5
0
end_operator
begin_operator
drop hoist1 crate1 depot0-1-2 depot0-1-1 depot0
1
1 2
4
0 4 -1 0
0 9 0 1
0 15 -1 0
0 17 1 6
0
end_operator
begin_operator
drop hoist1 crate1 depot0-1-2 depot0-2-2 depot0
1
1 5
4
0 4 -1 0
0 9 0 1
0 15 -1 0
0 17 1 6
0
end_operator
begin_operator
drop hoist1 crate1 depot0-2-1 depot0-1-1 depot0
1
1 2
4
0 4 -1 0
0 10 0 1
0 15 -1 0
0 17 1 7
0
end_operator
begin_operator
drop hoist1 crate1 depot0-2-1 depot0-2-2 depot0
1
1 5
4
0 4 -1 0
0 10 0 1
0 15 -1 0
0 17 1 7
0
end_operator
begin_operator
drop hoist1 crate1 depot0-2-1 loadarea depot0
1
1 6
4
0 4 -1 0
0 10 0 1
0 15 -1 0
0 17 1 7
0
end_operator
begin_operator
drop hoist1 crate1 depot0-2-2 depot0-1-2 depot0
1
1 3
4
0 4 -1 0
0 11 0 1
0 15 -1 0
0 17 1 8
0
end_operator
begin_operator
drop hoist1 crate1 depot0-2-2 depot0-2-1 depot0
1
1 4
4
0 4 -1 0
0 11 0 1
0 15 -1 0
0 17 1 8
0
end_operator
begin_operator
drop hoist2 crate0 container-0-0 loadarea container0
1
2 6
4
0 5 -1 0
0 6 0 1
0 12 -1 0
0 16 2 3
0
end_operator
begin_operator
drop hoist2 crate0 container-0-1 loadarea container0
1
2 6
4
0 5 -1 0
0 7 0 1
0 12 -1 0
0 16 2 4
0
end_operator
begin_operator
drop hoist2 crate0 depot0-1-1 depot0-1-2 depot0
1
2 3
4
0 5 -1 0
0 8 0 1
0 13 -1 0
0 16 2 5
0
end_operator
begin_operator
drop hoist2 crate0 depot0-1-1 depot0-2-1 depot0
1
2 4
4
0 5 -1 0
0 8 0 1
0 13 -1 0
0 16 2 5
0
end_operator
begin_operator
drop hoist2 crate0 depot0-1-2 depot0-1-1 depot0
1
2 2
4
0 5 -1 0
0 9 0 1
0 13 -1 0
0 16 2 6
0
end_operator
begin_operator
drop hoist2 crate0 depot0-1-2 depot0-2-2 depot0
1
2 5
4
0 5 -1 0
0 9 0 1
0 13 -1 0
0 16 2 6
0
end_operator
begin_operator
drop hoist2 crate0 depot0-2-1 depot0-1-1 depot0
1
2 2
4
0 5 -1 0
0 10 0 1
0 13 -1 0
0 16 2 7
0
end_operator
begin_operator
drop hoist2 crate0 depot0-2-1 depot0-2-2 depot0
1
2 5
4
0 5 -1 0
0 10 0 1
0 13 -1 0
0 16 2 7
0
end_operator
begin_operator
drop hoist2 crate0 depot0-2-1 loadarea depot0
1
2 6
4
0 5 -1 0
0 10 0 1
0 13 -1 0
0 16 2 7
0
end_operator
begin_operator
drop hoist2 crate0 depot0-2-2 depot0-1-2 depot0
1
2 3
4
0 5 -1 0
0 11 0 1
0 13 -1 0
0 16 2 8
0
end_operator
begin_operator
drop hoist2 crate0 depot0-2-2 depot0-2-1 depot0
1
2 4
4
0 5 -1 0
0 11 0 1
0 13 -1 0
0 16 2 8
0
end_operator
begin_operator
drop hoist2 crate1 container-0-0 loadarea container0
1
2 6
4
0 5 -1 0
0 6 0 1
0 14 -1 0
0 17 2 3
0
end_operator
begin_operator
drop hoist2 crate1 container-0-1 loadarea container0
1
2 6
4
0 5 -1 0
0 7 0 1
0 14 -1 0
0 17 2 4
0
end_operator
begin_operator
drop hoist2 crate1 depot0-1-1 depot0-1-2 depot0
1
2 3
4
0 5 -1 0
0 8 0 1
0 15 -1 0
0 17 2 5
0
end_operator
begin_operator
drop hoist2 crate1 depot0-1-1 depot0-2-1 depot0
1
2 4
4
0 5 -1 0
0 8 0 1
0 15 -1 0
0 17 2 5
0
end_operator
begin_operator
drop hoist2 crate1 depot0-1-2 depot0-1-1 depot0
1
2 2
4
0 5 -1 0
0 9 0 1
0 15 -1 0
0 17 2 6
0
end_operator
begin_operator
drop hoist2 crate1 depot0-1-2 depot0-2-2 depot0
1
2 5
4
0 5 -1 0
0 9 0 1
0 15 -1 0
0 17 2 6
0
end_operator
begin_operator
drop hoist2 crate1 depot0-2-1 depot0-1-1 depot0
1
2 2
4
0 5 -1 0
0 10 0 1
0 15 -1 0
0 17 2 7
0
end_operator
begin_operator
drop hoist2 crate1 depot0-2-1 depot0-2-2 depot0
1
2 5
4
0 5 -1 0
0 10 0 1
0 15 -1 0
0 17 2 7
0
end_operator
begin_operator
drop hoist2 crate1 depot0-2-1 loadarea depot0
1
2 6
4
0 5 -1 0
0 10 0 1
0 15 -1 0
0 17 2 7
0
end_operator
begin_operator
drop hoist2 crate1 depot0-2-2 depot0-1-2 depot0
1
2 3
4
0 5 -1 0
0 11 0 1
0 15 -1 0
0 17 2 8
0
end_operator
begin_operator
drop hoist2 crate1 depot0-2-2 depot0-2-1 depot0
1
2 4
4
0 5 -1 0
0 11 0 1
0 15 -1 0
0 17 2 8
0
end_operator
begin_operator
go-in hoist0 loadarea container-0-0
0
2
0 0 6 0
0 6 0 1
0
end_operator
begin_operator
go-in hoist0 loadarea container-0-1
0
2
0 0 6 1
0 7 0 1
0
end_operator
begin_operator
go-in hoist0 loadarea depot0-2-1
0
2
0 0 6 4
0 10 0 1
0
end_operator
begin_operator
go-in hoist1 loadarea container-0-0
0
2
0 1 6 0
0 6 0 1
0
end_operator
begin_operator
go-in hoist1 loadarea container-0-1
0
2
0 1 6 1
0 7 0 1
0
end_operator
begin_operator
go-in hoist1 loadarea depot0-2-1
0
2
0 1 6 4
0 10 0 1
0
end_operator
begin_operator
go-in hoist2 loadarea container-0-0
0
2
0 2 6 0
0 6 0 1
0
end_operator
begin_operator
go-in hoist2 loadarea container-0-1
0
2
0 2 6 1
0 7 0 1
0
end_operator
begin_operator
go-in hoist2 loadarea depot0-2-1
0
2
0 2 6 4
0 10 0 1
0
end_operator
begin_operator
go-out hoist0 container-0-0 loadarea
0
2
0 0 0 6
0 6 -1 0
0
end_operator
begin_operator
go-out hoist0 container-0-1 loadarea
0
2
0 0 1 6
0 7 -1 0
0
end_operator
begin_operator
go-out hoist0 depot0-2-1 loadarea
0
2
0 0 4 6
0 10 -1 0
0
end_operator
begin_operator
go-out hoist1 container-0-0 loadarea
0
2
0 1 0 6
0 6 -1 0
0
end_operator
begin_operator
go-out hoist1 container-0-1 loadarea
0
2
0 1 1 6
0 7 -1 0
0
end_operator
begin_operator
go-out hoist1 depot0-2-1 loadarea
0
2
0 1 4 6
0 10 -1 0
0
end_operator
begin_operator
go-out hoist2 container-0-0 loadarea
0
2
0 2 0 6
0 6 -1 0
0
end_operator
begin_operator
go-out hoist2 container-0-1 loadarea
0
2
0 2 1 6
0 7 -1 0
0
end_operator
begin_operator
go-out hoist2 depot0-2-1 loadarea
0
2
0 2 4 6
0 10 -1 0
0
end_operator
begin_operator
lift hoist0 crate0 container-0-0 loadarea container0
1
0 6
4
0 3 0 1
0 6 -1 0
0 12 -1 1
0 16 3 0
0
end_operator
begin_operator
lift hoist0 crate0 container-0-1 loadarea container0
1
0 6
4
0 3 0 1
0 7 -1 0
0 12 -1 1
0 16 4 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-1-1 depot0-1-2 depot0
1
0 3
4
0 3 0 1
0 8 -1 0
0 13 -1 1
0 16 5 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-1-1 depot0-2-1 depot0
1
0 4
4
0 3 0 1
0 8 -1 0
0 13 -1 1
0 16 5 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-1-2 depot0-1-1 depot0
1
0 2
4
0 3 0 1
0 9 -1 0
0 13 -1 1
0 16 6 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-1-2 depot0-2-2 depot0
1
0 5
4
0 3 0 1
0 9 -1 0
0 13 -1 1
0 16 6 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-2-1 depot0-1-1 depot0
1
0 2
4
0 3 0 1
0 10 -1 0
0 13 -1 1
0 16 7 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-2-1 depot0-2-2 depot0
1
0 5
4
0 3 0 1
0 10 -1 0
0 13 -1 1
0 16 7 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-2-1 loadarea depot0
1
0 6
4
0 3 0 1
0 10 -1 0
0 13 -1 1
0 16 7 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-2-2 depot0-1-2 depot0
1
0 3
4
0 3 0 1
0 11 -1 0
0 13 -1 1
0 16 8 0
0
end_operator
begin_operator
lift hoist0 crate0 depot0-2-2 depot0-2-1 depot0
1
0 4
4
0 3 0 1
0 11 -1 0
0 13 -1 1
0 16 8 0
0
end_operator
begin_operator
lift hoist0 crate1 container-0-0 loadarea container0
1
0 6
4
0 3 0 1
0 6 -1 0
0 14 -1 1
0 17 3 0
0
end_operator
begin_operator
lift hoist0 crate1 container-0-1 loadarea container0
1
0 6
4
0 3 0 1
0 7 -1 0
0 14 -1 1
0 17 4 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-1-1 depot0-1-2 depot0
1
0 3
4
0 3 0 1
0 8 -1 0
0 15 -1 1
0 17 5 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-1-1 depot0-2-1 depot0
1
0 4
4
0 3 0 1
0 8 -1 0
0 15 -1 1
0 17 5 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-1-2 depot0-1-1 depot0
1
0 2
4
0 3 0 1
0 9 -1 0
0 15 -1 1
0 17 6 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-1-2 depot0-2-2 depot0
1
0 5
4
0 3 0 1
0 9 -1 0
0 15 -1 1
0 17 6 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-2-1 depot0-1-1 depot0
1
0 2
4
0 3 0 1
0 10 -1 0
0 15 -1 1
0 17 7 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-2-1 depot0-2-2 depot0
1
0 5
4
0 3 0 1
0 10 -1 0
0 15 -1 1
0 17 7 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-2-1 loadarea depot0
1
0 6
4
0 3 0 1
0 10 -1 0
0 15 -1 1
0 17 7 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-2-2 depot0-1-2 depot0
1
0 3
4
0 3 0 1
0 11 -1 0
0 15 -1 1
0 17 8 0
0
end_operator
begin_operator
lift hoist0 crate1 depot0-2-2 depot0-2-1 depot0
1
0 4
4
0 3 0 1
0 11 -1 0
0 15 -1 1
0 17 8 0
0
end_operator
begin_operator
lift hoist1 crate0 container-0-0 loadarea container0
1
1 6
4
0 4 0 1
0 6 -1 0
0 12 -1 1
0 16 3 1
0
end_operator
begin_operator
lift hoist1 crate0 container-0-1 loadarea container0
1
1 6
4
0 4 0 1
0 7 -1 0
0 12 -1 1
0 16 4 1
0
end_operator
begin_operator
lift hoist1 crate0 depot0-1-1 depot0-1-2 depot0
1
1 3
4
0 4 0 1
0 8 -1 0
0 13 -1 1
0 16 5 1
0
end_operator
begin_operator
lift hoist1 crate0 depot0-1-1 depot0-2-1 depot0
1
1 4
4
0 4 0 1
0 8 -1 0
0 13 -1 1
0 16 5 1
0
end_operator
begin_operator
lift hoist1 crate0 depot0-1-2 depot0-1-1 depot0
1
1 2
4
0 4 0 1
0 9 -1 0
0 13 -1 1
0 16 6 1
0
end_operator
begin_operator
lift hoist1 crate0 depot0-1-2 depot0-2-2 depot0
1
1 5
4
0 4 0 1
0 9 -1 0
0 13 -1 1
0 16 6 1
0
end_operator
begin_operator
lift hoist1 crate0 depot0-2-1 depot0-1-1 depot0
1
1 2
4
0 4 0 1
0 10 -1 0
0 13 -1 1
0 16 7 1
0
end_operator
begin_operator
lift hoist1 crate0 depot0-2-1 depot0-2-2 depot0
1
1 5
4
0 4 0 1
0 10 -1 0
0 13 -1 1
0 16 7 1
0
end_operator
begin_operator
lift hoist1 crate0 depot0-2-1 loadarea depot0
1
1 6
4
0 4 0 1
0 10 -1 0
0 13 -1 1
0 16 7 1
0
end_operator
begin_operator
lift hoist1 crate0 depot0-2-2 depot0-1-2 depot0
1
1 3
4
0 4 0 1
0 11 -1 0
0 13 -1 1
0 16 8 1
0
end_operator
begin_operator
lift hoist1 crate0 depot0-2-2 depot0-2-1 depot0
1
1 4
4
0 4 0 1
0 11 -1 0
0 13 -1 1
0 16 8 1
0
end_operator
begin_operator
lift hoist1 crate1 container-0-0 loadarea container0
1
1 6
4
0 4 0 1
0 6 -1 0
0 14 -1 1
0 17 3 1
0
end_operator
begin_operator
lift hoist1 crate1 container-0-1 loadarea container0
1
1 6
4
0 4 0 1
0 7 -1 0
0 14 -1 1
0 17 4 1
0
end_operator
begin_operator
lift hoist1 crate1 depot0-1-1 depot0-1-2 depot0
1
1 3
4
0 4 0 1
0 8 -1 0
0 15 -1 1
0 17 5 1
0
end_operator
begin_operator
lift hoist1 crate1 depot0-1-1 depot0-2-1 depot0
1
1 4
4
0 4 0 1
0 8 -1 0
0 15 -1 1
0 17 5 1
0
end_operator
begin_operator
lift hoist1 crate1 depot0-1-2 depot0-1-1 depot0
1
1 2
4
0 4 0 1
0 9 -1 0
0 15 -1 1
0 17 6 1
0
end_operator
begin_operator
lift hoist1 crate1 depot0-1-2 depot0-2-2 depot0
1
1 5
4
0 4 0 1
0 9 -1 0
0 15 -1 1
0 17 6 1
0
end_operator
begin_operator
lift hoist1 crate1 depot0-2-1 depot0-1-1 depot0
1
1 2
4
0 4 0 1
0 10 -1 0
0 15 -1 1
0 17 7 1
0
end_operator
begin_operator
lift hoist1 crate1 depot0-2-1 depot0-2-2 depot0
1
1 5
4
0 4 0 1
0 10 -1 0
0 15 -1 1
0 17 7 1
0
end_operator
begin_operator
lift hoist1 crate1 depot0-2-1 loadarea depot0
1
1 6
4
0 4 0 1
0 10 -1 0
0 15 -1 1
0 17 7 1
0
end_operator
begin_operator
lift hoist1 crate1 depot0-2-2 depot0-1-2 depot0
1
1 3
4
0 4 0 1
0 11 -1 0
0 15 -1 1
0 17 8 1
0
end_operator
begin_operator
lift hoist1 crate1 depot0-2-2 depot0-2-1 depot0
1
1 4
4
0 4 0 1
0 11 -1 0
0 15 -1 1
0 17 8 1
0
end_operator
begin_operator
lift hoist2 crate0 container-0-0 loadarea container0
1
2 6
4
0 5 0 1
0 6 -1 0
0 12 -1 1
0 16 3 2
0
end_operator
begin_operator
lift hoist2 crate0 container-0-1 loadarea container0
1
2 6
4
0 5 0 1
0 7 -1 0
0 12 -1 1
0 16 4 2
0
end_operator
begin_operator
lift hoist2 crate0 depot0-1-1 depot0-1-2 depot0
1
2 3
4
0 5 0 1
0 8 -1 0
0 13 -1 1
0 16 5 2
0
end_operator
begin_operator
lift hoist2 crate0 depot0-1-1 depot0-2-1 depot0
1
2 4
4
0 5 0 1
0 8 -1 0
0 13 -1 1
0 16 5 2
0
end_operator
begin_operator
lift hoist2 crate0 depot0-1-2 depot0-1-1 depot0
1
2 2
4
0 5 0 1
0 9 -1 0
0 13 -1 1
0 16 6 2
0
end_operator
begin_operator
lift hoist2 crate0 depot0-1-2 depot0-2-2 depot0
1
2 5
4
0 5 0 1
0 9 -1 0
0 13 -1 1
0 16 6 2
0
end_operator
begin_operator
lift hoist2 crate0 depot0-2-1 depot0-1-1 depot0
1
2 2
4
0 5 0 1
0 10 -1 0
0 13 -1 1
0 16 7 2
0
end_operator
begin_operator
lift hoist2 crate0 depot0-2-1 depot0-2-2 depot0
1
2 5
4
0 5 0 1
0 10 -1 0
0 13 -1 1
0 16 7 2
0
end_operator
begin_operator
lift hoist2 crate0 depot0-2-1 loadarea depot0
1
2 6
4
0 5 0 1
0 10 -1 0
0 13 -1 1
0 16 7 2
0
end_operator
begin_operator
lift hoist2 crate0 depot0-2-2 depot0-1-2 depot0
1
2 3
4
0 5 0 1
0 11 -1 0
0 13 -1 1
0 16 8 2
0
end_operator
begin_operator
lift hoist2 crate0 depot0-2-2 depot0-2-1 depot0
1
2 4
4
0 5 0 1
0 11 -1 0
0 13 -1 1
0 16 8 2
0
end_operator
begin_operator
lift hoist2 crate1 container-0-0 loadarea container0
1
2 6
4
0 5 0 1
0 6 -1 0
0 14 -1 1
0 17 3 2
0
end_operator
begin_operator
lift hoist2 crate1 container-0-1 loadarea container0
1
2 6
4
0 5 0 1
0 7 -1 0
0 14 -1 1
0 17 4 2
0
end_operator
begin_operator
lift hoist2 crate1 depot0-1-1 depot0-1-2 depot0
1
2 3
4
0 5 0 1
0 8 -1 0
0 15 -1 1
0 17 5 2
0
end_operator
begin_operator
lift hoist2 crate1 depot0-1-1 depot0-2-1 depot0
1
2 4
4
0 5 0 1
0 8 -1 0
0 15 -1 1
0 17 5 2
0
end_operator
begin_operator
lift hoist2 crate1 depot0-1-2 depot0-1-1 depot0
1
2 2
4
0 5 0 1
0 9 -1 0
0 15 -1 1
0 17 6 2
0
end_operator
begin_operator
lift hoist2 crate1 depot0-1-2 depot0-2-2 depot0
1
2 5
4
0 5 0 1
0 9 -1 0
0 15 -1 1
0 17 6 2
0
end_operator
begin_operator
lift hoist2 crate1 depot0-2-1 depot0-1-1 depot0
1
2 2
4
0 5 0 1
0 10 -1 0
0 15 -1 1
0 17 7 2
0
end_operator
begin_operator
lift hoist2 crate1 depot0-2-1 depot0-2-2 depot0
1
2 5
4
0 5 0 1
0 10 -1 0
0 15 -1 1
0 17 7 2
0
end_operator
begin_operator
lift hoist2 crate1 depot0-2-1 loadarea depot0
1
2 6
4
0 5 0 1
0 10 -1 0
0 15 -1 1
0 17 7 2
0
end_operator
begin_operator
lift hoist2 crate1 depot0-2-2 depot0-1-2 depot0
1
2 3
4
0 5 0 1
0 11 -1 0
0 15 -1 1
0 17 8 2
0
end_operator
begin_operator
lift hoist2 crate1 depot0-2-2 depot0-2-1 depot0
1
2 4
4
0 5 0 1
0 11 -1 0
0 15 -1 1
0 17 8 2
0
end_operator
begin_operator
move hoist0 depot0-1-1 depot0-1-2
0
3
0 0 2 3
0 8 -1 0
0 9 0 1
0
end_operator
begin_operator
move hoist0 depot0-1-1 depot0-2-1
0
3
0 0 2 4
0 8 -1 0
0 10 0 1
0
end_operator
begin_operator
move hoist0 depot0-1-2 depot0-1-1
0
3
0 0 3 2
0 8 0 1
0 9 -1 0
0
end_operator
begin_operator
move hoist0 depot0-1-2 depot0-2-2
0
3
0 0 3 5
0 9 -1 0
0 11 0 1
0
end_operator
begin_operator
move hoist0 depot0-2-1 depot0-1-1
0
3
0 0 4 2
0 8 0 1
0 10 -1 0
0
end_operator
begin_operator
move hoist0 depot0-2-1 depot0-2-2
0
3
0 0 4 5
0 10 -1 0
0 11 0 1
0
end_operator
begin_operator
move hoist0 depot0-2-2 depot0-1-2
0
3
0 0 5 3
0 9 0 1
0 11 -1 0
0
end_operator
begin_operator
move hoist0 depot0-2-2 depot0-2-1
0
3
0 0 5 4
0 10 0 1
0 11 -1 0
0
end_operator
begin_operator
move hoist1 depot0-1-1 depot0-1-2
0
3
0 1 2 3
0 8 -1 0
0 9 0 1
0
end_operator
begin_operator
move hoist1 depot0-1-1 depot0-2-1
0
3
0 1 2 4
0 8 -1 0
0 10 0 1
0
end_operator
begin_operator
move hoist1 depot0-1-2 depot0-1-1
0
3
0 1 3 2
0 8 0 1
0 9 -1 0
0
end_operator
begin_operator
move hoist1 depot0-1-2 depot0-2-2
0
3
0 1 3 5
0 9 -1 0
0 11 0 1
0
end_operator
begin_operator
move hoist1 depot0-2-1 depot0-1-1
0
3
0 1 4 2
0 8 0 1
0 10 -1 0
0
end_operator
begin_operator
move hoist1 depot0-2-1 depot0-2-2
0
3
0 1 4 5
0 10 -1 0
0 11 0 1
0
end_operator
begin_operator
move hoist1 depot0-2-2 depot0-1-2
0
3
0 1 5 3
0 9 0 1
0 11 -1 0
0
end_operator
begin_operator
move hoist1 depot0-2-2 depot0-2-1
0
3
0 1 5 4
0 10 0 1
0 11 -1 0
0
end_operator
begin_operator
move hoist2 depot0-1-1 depot0-1-2
0
3
0 2 2 3
0 8 -1 0
0 9 0 1
0
end_operator
begin_operator
move hoist2 depot0-1-1 depot0-2-1
0
3
0 2 2 4
0 8 -1 0
0 10 0 1
0
end_operator
begin_operator
move hoist2 depot0-1-2 depot0-1-1
0
3
0 2 3 2
0 8 0 1
0 9 -1 0
0
end_operator
begin_operator
move hoist2 depot0-1-2 depot0-2-2
0
3
0 2 3 5
0 9 -1 0
0 11 0 1
0
end_operator
begin_operator
move hoist2 depot0-2-1 depot0-1-1
0
3
0 2 4 2
0 8 0 1
0 10 -1 0
0
end_operator
begin_operator
move hoist2 depot0-2-1 depot0-2-2
0
3
0 2 4 5
0 10 -1 0
0 11 0 1
0
end_operator
begin_operator
move hoist2 depot0-2-2 depot0-1-2
0
3
0 2 5 3
0 9 0 1
0 11 -1 0
0
end_operator
begin_operator
move hoist2 depot0-2-2 depot0-2-1
0
3
0 2 5 4
0 10 0 1
0 11 -1 0
0
end_operator
0
