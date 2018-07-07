import os

with open("./simple_graph.txt", "r") as f:
    N = int(f.readline())
    edges = []
    while True:
        line = f.readline().strip()
        if line == None or len(line) == 0:
            break
        line = line.split(" ")
        edges.append([int(line[0]), int(line[1])])

con = []
for i in range(N):
    con.append([])
for edge in edges:
    con[edge[0]].append(edge[1])

"""
for i in range(N):
    active.append(N)
"""

trimmed_num = 0
trimmed = []
for i in range(N):
    trimmed.append(False)

it = 0 

while True:

    it += 1

    degree = []
    newly_trimmed = 0
    for i in range(N):
        degree.append(0)
    
    for i in range(N):
        if trimmed[i] == False:
            for j in con[i]:
                degree[j] += 1

    for i in range(N):
        if trimmed[i] == False and degree[i] < 2:
            trimmed[i] = True
            newly_trimmed += 1

    if newly_trimmed == 0:
        break
    trimmed_num += newly_trimmed
    print "iter =", it, " trimmed = ", trimmed_num
    
print N - trimmed_num
