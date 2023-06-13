begin_version
3
end_version
begin_metric
0
end_metric
3
begin_variable
var0
-1
4
Atom at(person1, city0)
Atom at(person1, city1)
Atom at(person1, city2)
Atom in(person1, plane1)
end_variable
begin_variable
var1
-1
3
Atom at(plane1, city0)
Atom at(plane1, city1)
Atom at(plane1, city2)
end_variable
begin_variable
var2
-1
7
Atom fuel-level(plane1, fl0)
Atom fuel-level(plane1, fl1)
Atom fuel-level(plane1, fl2)
Atom fuel-level(plane1, fl3)
Atom fuel-level(plane1, fl4)
Atom fuel-level(plane1, fl5)
Atom fuel-level(plane1, fl6)
end_variable
3
begin_mutex_group
4
0 0
0 1
0 2
0 3
end_mutex_group
begin_mutex_group
3
1 0
1 1
1 2
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
begin_state
2
0
2
end_state
begin_goal
1
0 1
end_goal
123
begin_operator
board person1 plane1 city0
1
1 0
1
0 0 0 3
0
end_operator
begin_operator
board person1 plane1 city1
1
1 1
1
0 0 1 3
0
end_operator
begin_operator
board person1 plane1 city2
1
1 2
1
0 0 2 3
0
end_operator
begin_operator
debark person1 plane1 city0
1
1 0
1
0 0 3 0
0
end_operator
begin_operator
debark person1 plane1 city1
1
1 1
1
0 0 3 1
0
end_operator
begin_operator
debark person1 plane1 city2
1
1 2
1
0 0 3 2
0
end_operator
begin_operator
fly plane1 city0 city0 fl1 fl0
1
1 0
1
0 2 1 0
0
end_operator
begin_operator
fly plane1 city0 city0 fl2 fl1
1
1 0
1
0 2 2 1
0
end_operator
begin_operator
fly plane1 city0 city0 fl3 fl2
1
1 0
1
0 2 3 2
0
end_operator
begin_operator
fly plane1 city0 city0 fl4 fl3
1
1 0
1
0 2 4 3
0
end_operator
begin_operator
fly plane1 city0 city0 fl5 fl4
1
1 0
1
0 2 5 4
0
end_operator
begin_operator
fly plane1 city0 city0 fl6 fl5
1
1 0
1
0 2 6 5
0
end_operator
begin_operator
fly plane1 city0 city1 fl1 fl0
0
2
0 1 0 1
0 2 1 0
0
end_operator
begin_operator
fly plane1 city0 city1 fl2 fl1
0
2
0 1 0 1
0 2 2 1
0
end_operator
begin_operator
fly plane1 city0 city1 fl3 fl2
0
2
0 1 0 1
0 2 3 2
0
end_operator
begin_operator
fly plane1 city0 city1 fl4 fl3
0
2
0 1 0 1
0 2 4 3
0
end_operator
begin_operator
fly plane1 city0 city1 fl5 fl4
0
2
0 1 0 1
0 2 5 4
0
end_operator
begin_operator
fly plane1 city0 city1 fl6 fl5
0
2
0 1 0 1
0 2 6 5
0
end_operator
begin_operator
fly plane1 city0 city2 fl1 fl0
0
2
0 1 0 2
0 2 1 0
0
end_operator
begin_operator
fly plane1 city0 city2 fl2 fl1
0
2
0 1 0 2
0 2 2 1
0
end_operator
begin_operator
fly plane1 city0 city2 fl3 fl2
0
2
0 1 0 2
0 2 3 2
0
end_operator
begin_operator
fly plane1 city0 city2 fl4 fl3
0
2
0 1 0 2
0 2 4 3
0
end_operator
begin_operator
fly plane1 city0 city2 fl5 fl4
0
2
0 1 0 2
0 2 5 4
0
end_operator
begin_operator
fly plane1 city0 city2 fl6 fl5
0
2
0 1 0 2
0 2 6 5
0
end_operator
begin_operator
fly plane1 city1 city0 fl1 fl0
0
2
0 1 1 0
0 2 1 0
0
end_operator
begin_operator
fly plane1 city1 city0 fl2 fl1
0
2
0 1 1 0
0 2 2 1
0
end_operator
begin_operator
fly plane1 city1 city0 fl3 fl2
0
2
0 1 1 0
0 2 3 2
0
end_operator
begin_operator
fly plane1 city1 city0 fl4 fl3
0
2
0 1 1 0
0 2 4 3
0
end_operator
begin_operator
fly plane1 city1 city0 fl5 fl4
0
2
0 1 1 0
0 2 5 4
0
end_operator
begin_operator
fly plane1 city1 city0 fl6 fl5
0
2
0 1 1 0
0 2 6 5
0
end_operator
begin_operator
fly plane1 city1 city1 fl1 fl0
1
1 1
1
0 2 1 0
0
end_operator
begin_operator
fly plane1 city1 city1 fl2 fl1
1
1 1
1
0 2 2 1
0
end_operator
begin_operator
fly plane1 city1 city1 fl3 fl2
1
1 1
1
0 2 3 2
0
end_operator
begin_operator
fly plane1 city1 city1 fl4 fl3
1
1 1
1
0 2 4 3
0
end_operator
begin_operator
fly plane1 city1 city1 fl5 fl4
1
1 1
1
0 2 5 4
0
end_operator
begin_operator
fly plane1 city1 city1 fl6 fl5
1
1 1
1
0 2 6 5
0
end_operator
begin_operator
fly plane1 city1 city2 fl1 fl0
0
2
0 1 1 2
0 2 1 0
0
end_operator
begin_operator
fly plane1 city1 city2 fl2 fl1
0
2
0 1 1 2
0 2 2 1
0
end_operator
begin_operator
fly plane1 city1 city2 fl3 fl2
0
2
0 1 1 2
0 2 3 2
0
end_operator
begin_operator
fly plane1 city1 city2 fl4 fl3
0
2
0 1 1 2
0 2 4 3
0
end_operator
begin_operator
fly plane1 city1 city2 fl5 fl4
0
2
0 1 1 2
0 2 5 4
0
end_operator
begin_operator
fly plane1 city1 city2 fl6 fl5
0
2
0 1 1 2
0 2 6 5
0
end_operator
begin_operator
fly plane1 city2 city0 fl1 fl0
0
2
0 1 2 0
0 2 1 0
0
end_operator
begin_operator
fly plane1 city2 city0 fl2 fl1
0
2
0 1 2 0
0 2 2 1
0
end_operator
begin_operator
fly plane1 city2 city0 fl3 fl2
0
2
0 1 2 0
0 2 3 2
0
end_operator
begin_operator
fly plane1 city2 city0 fl4 fl3
0
2
0 1 2 0
0 2 4 3
0
end_operator
begin_operator
fly plane1 city2 city0 fl5 fl4
0
2
0 1 2 0
0 2 5 4
0
end_operator
begin_operator
fly plane1 city2 city0 fl6 fl5
0
2
0 1 2 0
0 2 6 5
0
end_operator
begin_operator
fly plane1 city2 city1 fl1 fl0
0
2
0 1 2 1
0 2 1 0
0
end_operator
begin_operator
fly plane1 city2 city1 fl2 fl1
0
2
0 1 2 1
0 2 2 1
0
end_operator
begin_operator
fly plane1 city2 city1 fl3 fl2
0
2
0 1 2 1
0 2 3 2
0
end_operator
begin_operator
fly plane1 city2 city1 fl4 fl3
0
2
0 1 2 1
0 2 4 3
0
end_operator
begin_operator
fly plane1 city2 city1 fl5 fl4
0
2
0 1 2 1
0 2 5 4
0
end_operator
begin_operator
fly plane1 city2 city1 fl6 fl5
0
2
0 1 2 1
0 2 6 5
0
end_operator
begin_operator
fly plane1 city2 city2 fl1 fl0
1
1 2
1
0 2 1 0
0
end_operator
begin_operator
fly plane1 city2 city2 fl2 fl1
1
1 2
1
0 2 2 1
0
end_operator
begin_operator
fly plane1 city2 city2 fl3 fl2
1
1 2
1
0 2 3 2
0
end_operator
begin_operator
fly plane1 city2 city2 fl4 fl3
1
1 2
1
0 2 4 3
0
end_operator
begin_operator
fly plane1 city2 city2 fl5 fl4
1
1 2
1
0 2 5 4
0
end_operator
begin_operator
fly plane1 city2 city2 fl6 fl5
1
1 2
1
0 2 6 5
0
end_operator
begin_operator
refuel plane1 city0 fl0 fl1
1
1 0
1
0 2 0 1
0
end_operator
begin_operator
refuel plane1 city0 fl1 fl2
1
1 0
1
0 2 1 2
0
end_operator
begin_operator
refuel plane1 city0 fl2 fl3
1
1 0
1
0 2 2 3
0
end_operator
begin_operator
refuel plane1 city0 fl3 fl4
1
1 0
1
0 2 3 4
0
end_operator
begin_operator
refuel plane1 city0 fl4 fl5
1
1 0
1
0 2 4 5
0
end_operator
begin_operator
refuel plane1 city0 fl5 fl6
1
1 0
1
0 2 5 6
0
end_operator
begin_operator
refuel plane1 city1 fl0 fl1
1
1 1
1
0 2 0 1
0
end_operator
begin_operator
refuel plane1 city1 fl1 fl2
1
1 1
1
0 2 1 2
0
end_operator
begin_operator
refuel plane1 city1 fl2 fl3
1
1 1
1
0 2 2 3
0
end_operator
begin_operator
refuel plane1 city1 fl3 fl4
1
1 1
1
0 2 3 4
0
end_operator
begin_operator
refuel plane1 city1 fl4 fl5
1
1 1
1
0 2 4 5
0
end_operator
begin_operator
refuel plane1 city1 fl5 fl6
1
1 1
1
0 2 5 6
0
end_operator
begin_operator
refuel plane1 city2 fl0 fl1
1
1 2
1
0 2 0 1
0
end_operator
begin_operator
refuel plane1 city2 fl1 fl2
1
1 2
1
0 2 1 2
0
end_operator
begin_operator
refuel plane1 city2 fl2 fl3
1
1 2
1
0 2 2 3
0
end_operator
begin_operator
refuel plane1 city2 fl3 fl4
1
1 2
1
0 2 3 4
0
end_operator
begin_operator
refuel plane1 city2 fl4 fl5
1
1 2
1
0 2 4 5
0
end_operator
begin_operator
refuel plane1 city2 fl5 fl6
1
1 2
1
0 2 5 6
0
end_operator
begin_operator
zoom plane1 city0 city0 fl2 fl1 fl0
1
1 0
1
0 2 2 0
0
end_operator
begin_operator
zoom plane1 city0 city0 fl3 fl2 fl1
1
1 0
1
0 2 3 1
0
end_operator
begin_operator
zoom plane1 city0 city0 fl4 fl3 fl2
1
1 0
1
0 2 4 2
0
end_operator
begin_operator
zoom plane1 city0 city0 fl5 fl4 fl3
1
1 0
1
0 2 5 3
0
end_operator
begin_operator
zoom plane1 city0 city0 fl6 fl5 fl4
1
1 0
1
0 2 6 4
0
end_operator
begin_operator
zoom plane1 city0 city1 fl2 fl1 fl0
0
2
0 1 0 1
0 2 2 0
0
end_operator
begin_operator
zoom plane1 city0 city1 fl3 fl2 fl1
0
2
0 1 0 1
0 2 3 1
0
end_operator
begin_operator
zoom plane1 city0 city1 fl4 fl3 fl2
0
2
0 1 0 1
0 2 4 2
0
end_operator
begin_operator
zoom plane1 city0 city1 fl5 fl4 fl3
0
2
0 1 0 1
0 2 5 3
0
end_operator
begin_operator
zoom plane1 city0 city1 fl6 fl5 fl4
0
2
0 1 0 1
0 2 6 4
0
end_operator
begin_operator
zoom plane1 city0 city2 fl2 fl1 fl0
0
2
0 1 0 2
0 2 2 0
0
end_operator
begin_operator
zoom plane1 city0 city2 fl3 fl2 fl1
0
2
0 1 0 2
0 2 3 1
0
end_operator
begin_operator
zoom plane1 city0 city2 fl4 fl3 fl2
0
2
0 1 0 2
0 2 4 2
0
end_operator
begin_operator
zoom plane1 city0 city2 fl5 fl4 fl3
0
2
0 1 0 2
0 2 5 3
0
end_operator
begin_operator
zoom plane1 city0 city2 fl6 fl5 fl4
0
2
0 1 0 2
0 2 6 4
0
end_operator
begin_operator
zoom plane1 city1 city0 fl2 fl1 fl0
0
2
0 1 1 0
0 2 2 0
0
end_operator
begin_operator
zoom plane1 city1 city0 fl3 fl2 fl1
0
2
0 1 1 0
0 2 3 1
0
end_operator
begin_operator
zoom plane1 city1 city0 fl4 fl3 fl2
0
2
0 1 1 0
0 2 4 2
0
end_operator
begin_operator
zoom plane1 city1 city0 fl5 fl4 fl3
0
2
0 1 1 0
0 2 5 3
0
end_operator
begin_operator
zoom plane1 city1 city0 fl6 fl5 fl4
0
2
0 1 1 0
0 2 6 4
0
end_operator
begin_operator
zoom plane1 city1 city1 fl2 fl1 fl0
1
1 1
1
0 2 2 0
0
end_operator
begin_operator
zoom plane1 city1 city1 fl3 fl2 fl1
1
1 1
1
0 2 3 1
0
end_operator
begin_operator
zoom plane1 city1 city1 fl4 fl3 fl2
1
1 1
1
0 2 4 2
0
end_operator
begin_operator
zoom plane1 city1 city1 fl5 fl4 fl3
1
1 1
1
0 2 5 3
0
end_operator
begin_operator
zoom plane1 city1 city1 fl6 fl5 fl4
1
1 1
1
0 2 6 4
0
end_operator
begin_operator
zoom plane1 city1 city2 fl2 fl1 fl0
0
2
0 1 1 2
0 2 2 0
0
end_operator
begin_operator
zoom plane1 city1 city2 fl3 fl2 fl1
0
2
0 1 1 2
0 2 3 1
0
end_operator
begin_operator
zoom plane1 city1 city2 fl4 fl3 fl2
0
2
0 1 1 2
0 2 4 2
0
end_operator
begin_operator
zoom plane1 city1 city2 fl5 fl4 fl3
0
2
0 1 1 2
0 2 5 3
0
end_operator
begin_operator
zoom plane1 city1 city2 fl6 fl5 fl4
0
2
0 1 1 2
0 2 6 4
0
end_operator
begin_operator
zoom plane1 city2 city0 fl2 fl1 fl0
0
2
0 1 2 0
0 2 2 0
0
end_operator
begin_operator
zoom plane1 city2 city0 fl3 fl2 fl1
0
2
0 1 2 0
0 2 3 1
0
end_operator
begin_operator
zoom plane1 city2 city0 fl4 fl3 fl2
0
2
0 1 2 0
0 2 4 2
0
end_operator
begin_operator
zoom plane1 city2 city0 fl5 fl4 fl3
0
2
0 1 2 0
0 2 5 3
0
end_operator
begin_operator
zoom plane1 city2 city0 fl6 fl5 fl4
0
2
0 1 2 0
0 2 6 4
0
end_operator
begin_operator
zoom plane1 city2 city1 fl2 fl1 fl0
0
2
0 1 2 1
0 2 2 0
0
end_operator
begin_operator
zoom plane1 city2 city1 fl3 fl2 fl1
0
2
0 1 2 1
0 2 3 1
0
end_operator
begin_operator
zoom plane1 city2 city1 fl4 fl3 fl2
0
2
0 1 2 1
0 2 4 2
0
end_operator
begin_operator
zoom plane1 city2 city1 fl5 fl4 fl3
0
2
0 1 2 1
0 2 5 3
0
end_operator
begin_operator
zoom plane1 city2 city1 fl6 fl5 fl4
0
2
0 1 2 1
0 2 6 4
0
end_operator
begin_operator
zoom plane1 city2 city2 fl2 fl1 fl0
1
1 2
1
0 2 2 0
0
end_operator
begin_operator
zoom plane1 city2 city2 fl3 fl2 fl1
1
1 2
1
0 2 3 1
0
end_operator
begin_operator
zoom plane1 city2 city2 fl4 fl3 fl2
1
1 2
1
0 2 4 2
0
end_operator
begin_operator
zoom plane1 city2 city2 fl5 fl4 fl3
1
1 2
1
0 2 5 3
0
end_operator
begin_operator
zoom plane1 city2 city2 fl6 fl5 fl4
1
1 2
1
0 2 6 4
0
end_operator
0
