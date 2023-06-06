import json
import sys

j = []

with open(sys.argv[1], 'r') as branches_file:
    line = branches_file.readline()
    while line:
        if line[0] == '#':
            
            j_item = {'k': '', 'v': [], 's': [], 'p': []}
            line = branches_file.readline()

        if line == 'Remaining faults : \n':
            
            line = branches_file.readline()
            j_item['k'] = line[:-1]
            line = branches_file.readline()

        if line == 'Failed operators : \n':
            
            f = []
            line = branches_file.readline()
            while line != 'State : \n':
                f.append(line[:-1])
                line = branches_file.readline()
            j_item['v'] = f

        if line == 'State : \n':
            line = branches_file.readline()

            s = []
            while line != 'Plan : \n':
                s.append(line[:-1])
                line = branches_file.readline()
            j_item['s'] = s

        if line == 'Plan : \n':
            line = branches_file.readline()

            p = []
            while line[0] != '#' or line == None:
                p.append(line[:-1])
                line = branches_file.readline()
            j_item['p'] = p
            line = branches_file.readline()

        j.append(j_item)

for i in j:
    y = json.dumps(i)
    print(y)