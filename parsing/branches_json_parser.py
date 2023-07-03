import json
import sys

j = []

with open(sys.argv[1], 'r') as branches_file:
    line = branches_file.readline()
    while line:
        if line[0] == '#':
            j_item = {'k': '', 'forbidden': [], 'state': [], 'plan': []}
            line = branches_file.readline()

        if line == 'Remaining faults : \n':
            line = branches_file.readline()
            j_item['k'] = int(line[:-1])
            line = branches_file.readline()

        if line == 'Failed operators : \n':
            f = []
            line = branches_file.readline()
            while line != 'State : \n':
                f.append(line[:-1])
                line = branches_file.readline()
            j_item['forbidden'] = f

        if line == 'State : \n':
            line = branches_file.readline()
            s = []
            while line != 'Plan : \n':
                line = line.replace("(", " ")
                line = line.replace(")", "")
                line = line.replace(", ", " ")
                s.append(line[5:-1])
                line = branches_file.readline()
            j_item['state'] = s

        if line == 'Plan : \n':
            line = branches_file.readline()
            p = []
            while line[0] != '#' or line == None:
                p.append(line[:-1])
                line = branches_file.readline()
            j_item['plan'] = p

        j.append(j_item)

    j.pop(-1)


original_stdout = sys.stdout
with open('branches.json', 'w') as f:
    sys.stdout = f
    j_dump = json.dumps(j)
    print(j_dump)
    sys.stdout = original_stdout 


with open('branches.json', 'r') as f:
    j_read = json.loads(f.read())
    for i in j_read:
        print(i)
    sys.stdout = original_stdout
    