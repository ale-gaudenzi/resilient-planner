import json
import sys

j = []

with open(sys.argv[1], 'r') as branches_file:
    line = branches_file.readline()
    while line:

        if line[0] == '#':
            j_item = {'id': '', 'state': [], 'k': '', 'forbidden': [], 'action': ''}
            line = branches_file.readline()

        if line[:5] == 'Node:':
            j_item['id'] = line.split(' ')[1][:-1]
            s = []
            line = branches_file.readline()
            while line[0] != 'k':
                s.append(line.removeprefix('Atom ')[:-1])
                line = branches_file.readline()
            j_item['state'] = s

        if line[0] == 'k':
            a = line.split(' ')
            j_item['k'] = line.split(' ')[1][:-1]
            line = branches_file.readline()

        if line == 'deactivated_op: \n':
            line = branches_file.readline()
            f = []
            while line[:7] != 'Action:':
                f.append(line[:-1])
                line = branches_file.readline()
            j_item['forbidden'] = f

        if line[:7] == 'Action:':
            line = branches_file.readline()
            j_item['action'] = line[:-1]
            line = branches_file.readline()

        
        j.append(j_item)
    j.pop(-1)


original_stdout = sys.stdout
with open('policy.json', 'w') as f:
    sys.stdout = f
    j_dump = json.dumps(j)
    print(j_dump)
    sys.stdout = original_stdout 


with open('policy.json', 'r') as f:
    j_read = json.loads(f.read())
    for i in j_read:
        print(i)
    sys.stdout = original_stdout
    