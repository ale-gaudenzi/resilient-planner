begin_version
3.FOND
end_version
begin_metric
0
end_metric
16
begin_variable
var0
-1
3
Atom at(hoist0, container-0-0)
Atom at(hoist0, depot0-1-1)
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
Atom clear(depot0-1-1)
NegatedAtom clear(depot0-1-1)
end_variable
begin_variable
var4
-1
2
Atom drop(hoist0, crate0, container-0-0, loadarea, container0)
NegatedAtom drop(hoist0, crate0, container-0-0, loadarea, container0)
end_variable
begin_variable
var5
-1
2
Atom drop(hoist0, crate0, depot0-1-1, loadarea, depot0)
NegatedAtom drop(hoist0, crate0, depot0-1-1, loadarea, depot0)
end_variable
begin_variable
var6
-1
2
Atom faults_0()
Atom faults_1()
end_variable
begin_variable
var7
-1
2
Atom go-in(hoist0, loadarea, container-0-0)
NegatedAtom go-in(hoist0, loadarea, container-0-0)
end_variable
begin_variable
var8
-1
2
Atom go-in(hoist0, loadarea, depot0-1-1)
NegatedAtom go-in(hoist0, loadarea, depot0-1-1)
end_variable
begin_variable
var9
-1
2
Atom go-out(hoist0, container-0-0, loadarea)
NegatedAtom go-out(hoist0, container-0-0, loadarea)
end_variable
begin_variable
var10
-1
2
Atom go-out(hoist0, depot0-1-1, loadarea)
NegatedAtom go-out(hoist0, depot0-1-1, loadarea)
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
Atom lift(hoist0, crate0, container-0-0, loadarea, container0)
NegatedAtom lift(hoist0, crate0, container-0-0, loadarea, container0)
end_variable
begin_variable
var14
-1
2
Atom lift(hoist0, crate0, depot0-1-1, loadarea, depot0)
NegatedAtom lift(hoist0, crate0, depot0-1-1, loadarea, depot0)
end_variable
begin_variable
var15
-1
3
Atom lifting(hoist0, crate0)
Atom on(crate0, container-0-0)
Atom on(crate0, depot0-1-1)
end_variable
4
begin_mutex_group
3
0 0
0 1
0 2
end_mutex_group
begin_mutex_group
2
1 0
15 0
end_mutex_group
begin_mutex_group
2
6 0
6 1
end_mutex_group
begin_mutex_group
3
15 0
15 1
15 2
end_mutex_group
begin_state
1
0
1
1
1
1
0
1
1
1
1
0
1
1
1
1
end_state
begin_goal
1
12 0
end_goal
16
begin_operator
drop-0 hoist0 crate0 container-0-0 loadarea container0
5
0 2
2 0
4 1
6 0
15 0
2
4
0 1 -1 0
0 2 0 1
0 11 -1 0
0 15 0 1
2
0 4 1 0
0 6 0 1
0
end_operator
begin_operator
drop-0 hoist0 crate0 depot0-1-1 loadarea depot0
5
0 2
3 0
5 1
6 0
15 0
2
4
0 1 -1 0
0 3 0 1
0 12 -1 0
0 15 0 2
2
0 5 1 0
0 6 0 1
0
end_operator
begin_operator
drop-1 hoist0 crate0 container-0-0 loadarea container0
5
0 2
2 0
4 1
6 1
15 0
1
4
0 1 -1 0
0 2 0 1
0 11 -1 0
0 15 0 1
0
end_operator
begin_operator
drop-1 hoist0 crate0 depot0-1-1 loadarea depot0
5
0 2
3 0
5 1
6 1
15 0
1
4
0 1 -1 0
0 3 0 1
0 12 -1 0
0 15 0 2
0
end_operator
begin_operator
go-in-0 hoist0 loadarea container-0-0
4
0 2
2 0
6 0
7 1
2
2
0 0 2 0
0 2 0 1
2
0 6 0 1
0 7 1 0
0
end_operator
begin_operator
go-in-0 hoist0 loadarea depot0-1-1
4
0 2
3 0
6 0
8 1
2
2
0 0 2 1
0 3 0 1
2
0 6 0 1
0 8 1 0
0
end_operator
begin_operator
go-in-1 hoist0 loadarea container-0-0
4
0 2
2 0
6 1
7 1
1
2
0 0 2 0
0 2 0 1
0
end_operator
begin_operator
go-in-1 hoist0 loadarea depot0-1-1
4
0 2
3 0
6 1
8 1
1
2
0 0 2 1
0 3 0 1
0
end_operator
begin_operator
go-out-0 hoist0 container-0-0 loadarea
3
0 0
6 0
9 1
2
2
0 0 0 2
0 2 -1 0
2
0 6 0 1
0 9 1 0
0
end_operator
begin_operator
go-out-0 hoist0 depot0-1-1 loadarea
3
0 1
6 0
10 1
2
2
0 0 1 2
0 3 -1 0
2
0 6 0 1
0 10 1 0
0
end_operator
begin_operator
go-out-1 hoist0 container-0-0 loadarea
3
0 0
6 1
9 1
1
2
0 0 0 2
0 2 -1 0
0
end_operator
begin_operator
go-out-1 hoist0 depot0-1-1 loadarea
3
0 1
6 1
10 1
1
2
0 0 1 2
0 3 -1 0
0
end_operator
begin_operator
lift-0 hoist0 crate0 container-0-0 loadarea container0
5
0 2
1 0
6 0
13 1
15 1
2
4
0 1 0 1
0 2 -1 0
0 11 -1 1
0 15 1 0
2
0 6 0 1
0 13 1 0
0
end_operator
begin_operator
lift-0 hoist0 crate0 depot0-1-1 loadarea depot0
5
0 2
1 0
6 0
14 1
15 2
2
4
0 1 0 1
0 3 -1 0
0 12 -1 1
0 15 2 0
2
0 6 0 1
0 14 1 0
0
end_operator
begin_operator
lift-1 hoist0 crate0 container-0-0 loadarea container0
5
0 2
1 0
6 1
13 1
15 1
1
4
0 1 0 1
0 2 -1 0
0 11 -1 1
0 15 1 0
0
end_operator
begin_operator
lift-1 hoist0 crate0 depot0-1-1 loadarea depot0
5
0 2
1 0
6 1
14 1
15 2
1
4
0 1 0 1
0 3 -1 0
0 12 -1 1
0 15 2 0
0
end_operator
0
