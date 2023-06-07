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
                s.append(line[5:-1])
                line = branches_file.readline()
            j_item['s'] = s

        if line == 'Plan : \n':
            line = branches_file.readline()
            p = []
            while line[0] != '#' or line == None:
                p.append(line[:-1])
                line = branches_file.readline()
            j_item['p'] = p

        j.append(j_item)
    j.pop(-1)


original_stdout = sys.stdout
with open('branches.json', 'w') as f:
    sys.stdout = f
    for line in j:
        json_line = json.dumps(line)
        print(json_line)
    sys.stdout = original_stdout 



